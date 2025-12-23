#include <algorithms/quantize.hpp>
#include <core/core.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace dip;

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);
  spdlog::info("Image Quantization Test");

  std::string filename = argc > 1 ? argv[1] : "test.png";
  int levels = argc > 2 ? std::atoi(argv[2]) : 16;

  spdlog::info("Loading image: {}, levels: {}", filename, levels);

  auto img = ImageLoader::load_from_file(filename);
  if (!img || img->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  spdlog::info("Original image: {}x{}x{}", img->width(), img->height(),
               img->channels());

  try {
    auto result = algorithms::quantize(*img, levels);
    spdlog::info("Quantized image: {}x{}x{}", result.width(), result.height(),
                 result.channels());

    std::string output = "quantized_" + std::to_string(levels) + "_levels.ppm";
    if (image_saver::save_binary(result, output)) {
      spdlog::info("Saved result: {}", output);
    }
  } catch (const std::exception &e) {
    spdlog::error("Error: {}", e.what());
    return 1;
  }

  return 0;
}
