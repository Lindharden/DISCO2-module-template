#include "module.h"
#include "util.h"
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include "tensorflow/lite/interpreter.h"
#include "tensorflow/lite/kernels/register.h"
#include "tensorflow/lite/string_util.h"
#include "tensorflow/lite/examples/label_image/get_top_n.h"
#include "tensorflow/lite/model.h"

std::vector<std::string> load_labels(const std::string& labels_file) {
    std::ifstream file(labels_file.c_str());
    if (!file.is_open()) {
        fprintf(stderr, "unable to open label file\n");
        exit(-1);
    }
    std::string label_str;
    std::vector<std::string> labels;

    while (std::getline(file, label_str)) {
        if (label_str.size() > 0)
            labels.push_back(label_str);
    }
    file.close();
    return labels;
}

void print_label(const std::vector<std::pair<float, int>>& top_results, const std::vector<std::string>& labels) {
    for (const auto &result : top_results) {
        const float confidence = result.first;
        const int index = result.second;
        std::cout << "Label: " << labels[index] << " Confidence: " << confidence << std::endl;
    }
}

/* START MODULE IMPLEMENTATION */
void module() {
    const char* modelFileName = "models/mobilenet_v1_1.0_224_quant.tflite";
    const char* labelsFileName = "models/labels_mobilenet_quant_v1_224.txt";

    // Load Model
    std::unique_ptr<tflite::FlatBufferModel> model = tflite::FlatBufferModel::BuildFromFile(modelFileName);
    if (model == nullptr) {
        fprintf(stderr, "failed to load model\n");
        exit(-1);
    }

    // Initiate Interpreter
    std::unique_ptr<tflite::Interpreter> interpreter;
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder(*model.get(), resolver)(&interpreter);
    if (interpreter == nullptr) {
        fprintf(stderr, "Failed to initiate the interpreter\n");
        exit(-1);
    }

    if (interpreter->AllocateTensors() != kTfLiteOk) {
        fprintf(stderr, "Failed to allocate tensor\n");
        exit(-1);
    }

    // Configure the interpreter
    interpreter->SetAllowFp16PrecisionForFp32(true);
    interpreter->SetNumThreads(1);

    // Load Labels
    std::vector<std::string> labels = load_labels(labelsFileName);

    /* Get number of images in input batch */
    int num_images = get_input_num_images();

    /* Iterate through each image in the batch */
    for (int i = 0; i < num_images; ++i) {
        Metadata *input_meta = get_metadata(i);
        int height = input_meta->height;
        int width = input_meta->width;
        int channels = input_meta->channels;
        int timestamp = input_meta->timestamp;
        int bits_pixel = input_meta->bits_pixel;
        char *camera = input_meta->camera;

        unsigned char *input_image_data;
        size_t size = get_image_data(i, &input_image_data);

        // Get Input Tensor Dimensions
        int input = interpreter->inputs()[0];
        auto input_height = interpreter->tensor(input)->dims->data[1];
        auto input_width = interpreter->tensor(input)->dims->data[2];
        auto input_channels = interpreter->tensor(input)->dims->data[3];

        // Load Input Image (SOMETHING GOES WRONG HERE)
        cv::Mat image(height, width, (channels == 3) ? CV_8UC3 : CV_8UC1, input_image_data);
        if (image.empty()) {
            fprintf(stderr, "Failed to load image\n");
            exit(-1);
        }

        // Resize and copy image to input tensor
        cv::Mat resized_image;
        cv::resize(image, resized_image, cv::Size(input_width, input_height), cv::INTER_NEAREST);
        memcpy(interpreter->typed_input_tensor<unsigned char>(0), resized_image.data, resized_image.total() * resized_image.elemSize());

        // Inference
        interpreter->Invoke();

        // Get Output
        int output = interpreter->outputs()[0];
        TfLiteIntArray *output_dims = interpreter->tensor(output)->dims;
        auto output_size = output_dims->data[output_dims->size - 1];
        std::vector<std::pair<float, int>> top_results;
        float threshold = 0.01f;

        switch (interpreter->tensor(output)->type) {
            case kTfLiteFloat32:
                tflite::label_image::get_top_n<float>(interpreter->typed_output_tensor<float>(0), output_size, 1, threshold, &top_results, kTfLiteFloat32);
                break;
            case kTfLiteUInt8:
                tflite::label_image::get_top_n<uint8_t>(interpreter->typed_output_tensor<uint8_t>(0), output_size, 1, threshold, &top_results, kTfLiteUInt8);
                break;
            default:
                fprintf(stderr, "cannot handle output type\n");
                exit(-1);
        }

        // Print the classified label
        print_label(top_results, labels);

        /* Create image metadata before appending */
        Metadata new_meta = METADATA__INIT;
        new_meta.size = size;
        new_meta.width = width;
        new_meta.height = height;
        new_meta.channels = channels;
        new_meta.timestamp = timestamp;
        new_meta.bits_pixel = bits_pixel;
        new_meta.camera = camera;

        /* Append the image to the result batch */
        append_result_image(input_image_data, size, &new_meta);

        /* Remember to free any allocated memory */
        free(input_image_data);
    }
}
/* END MODULE IMPLEMENTATION */

/* Main function of module (NO NEED TO MODIFY) */
ImageBatch run(ImageBatch *input_batch, ModuleParameterList *module_parameter_list, int *ipc_error_pipe) {
    ImageBatch result_batch;
    result = &result_batch;
    input = input_batch;
    config = module_parameter_list;
    error_pipe = ipc_error_pipe;
    initialize();

    module();

    finalize();

    return result_batch;
}
