#include "module.h"
#include "util.h"
#include "edgetpu.h"
#include <tensorflow/lite/interpreter.h>
#include <tensorflow/lite/model.h>
#include <tensorflow/lite/kernels/register.h>
#include <tensorflow/lite/tools/gen_op_registration.h>
#include <iostream>
#include <opencv2/opencv.hpp>

/* Define custom error codes */
enum ERROR_CODE {
    MALLOC_ERR = 1,
    PLACEHOLDER = 2,
};

void softmax(std::vector<float>& input) {
    /* Apply softmax to convert logits to probabilities */
    float sum = 0.0f;
    for(auto& val : input) {
        val = std::exp(val);
        sum += val;
    }
    for(auto& val : input) {
        val /= sum;
    }
}

void prepareInputData(float* inputTensorData, ImageBatch input_data) {
    /* Prepare image data for inference */
    unsigned char * image_data;
    size_t image_size = get_image_data(1, &image_data);
    
    /* Create OpenCV Image object */
    cv::Mat image = cv::Mat(get_input_height(), get_input_width(), CV_8UC3, (void*)image_data); // CV_8UC3 indicates 8-bit unsigned integer with 3 channels (RBG)
    cv::cvtColor(image, image, cv::COLOR_RGB2BGR);

    /* Expected image size */
    int width = 224, height = 224, channels = 3;

    /* Resize image to expected image size for model */
    cv::Mat resizedImage;
    cv::resize(image, resizedImage, cv::Size(width, height), 0, 0, cv::INTER_LINEAR);

    /* Normalization of pixel values to a value between 0 and 1 - as required by the model https://www.kaggle.com/models/google/mobilenet-v3/frameworks/tfLite */
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            for (int c = 0; c < channels; ++c) {
                int idx = (y * width + x) * channels + c;
                inputTensorData[idx] = resizedImage.at<cv::Vec3b>(y, x)[c] / 255.0f;
            }
        }
    }
}

std::unique_ptr<tflite::Interpreter> interpreter;

/* START MODULE IMPLEMENTATION */
void module()
{
    /* Get metadata of input batch */
    int width = get_input_width();
    int height = get_input_height();
    int channels = get_input_channels();
    int num_images = get_input_num_images();
    
    /* Edge TPU context that we need to create for the Coral AI tpu */
    auto edgetpu_context = edgetpu::EdgeTpuManager::GetSingleton()->OpenDevice();

    if (!edgetpu_context) {
        std::cerr << "Failed to obtain Edge TPU context" << std::endl;
        return;
    }

    const char* model_path = "model/mobilenet.tflite"; // https://www.kaggle.com/models/google/mobilenet-v3/frameworks/tfLite
    auto model = tflite::FlatBufferModel::BuildFromFile(model_path);

    if (!model) {
        std::cerr << "Failed to load model" << std::endl;
        return;
    }

    /* Expected input image shape of 224 x 224 pixels */
    const int model_height = 224;
    const int model_width = 224;
    const int model_channels = 3;

    tflite::ops::builtin::BuiltinOpResolver resolver;

    /* Register the Edge TPU */
    resolver.AddCustom(edgetpu::kCustomOp, edgetpu::RegisterCustomOp());

    /* Build the interpreter with the Edge TPU context added above */
    tflite::InterpreterBuilder builder(*model, resolver);
    builder(&interpreter);
    if (!interpreter) {
        std::cerr << "Failed to create interpreter" << std::endl;
        return;
    }

    /* Bind the Edge TPU context to the interpreter */
    interpreter->SetExternalContext(kTfLiteEdgeTpuContext, edgetpu_context.get());
    interpreter->SetNumThreads(1); // When number of threads is 1 it becomes deterministic

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        std::cerr << "Failed to allocate tensors!" << std::endl;
        return;
    }

    /* Preprocess input data and set the input tensor */
    float* inputTensorData = interpreter->typed_input_tensor<float>(0);
    prepareInputData(inputTensorData, &input);

    /* Run inference */
    if (interpreter->Invoke() != kTfLiteOk) {
        std::cerr << "Failed to invoke interpreter" << std::endl;
        return;
    }

    /* Retrieve output data */
    float* outputData = interpreter->typed_output_tensor<float>(0);
    int outputSize = interpreter->tensor(interpreter->outputs()[0])->bytes / sizeof(float);
    std::vector<float> outputVector(outputData, outputData + outputSize);

    /* Apply softmax to the output vector to convert logits to probabilities */
    softmax(outputVector);

    /* Print the probabilities */
    for (size_t i = 0; i < outputVector.size(); ++i) {
        std::cout << "Probability of class " << i << ": " << outputVector[i] << std::endl;
    }
}
/* END MODULE IMPLEMENTATION */

/* Main function of module (NO NEED TO MODIFY) */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list, int *ipc_error_pipe)
{
    ImageBatch result_batch;
    result = &result_batch;
    result->batch_size = 0;
    input = input_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;

    module();

    finalize();

    return result_batch;
}