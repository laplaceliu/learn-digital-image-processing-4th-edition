#include <algorithms/downsample.hpp>
#include <algorithms/nearest_neighbor_zoom.hpp>
#include <algorithms/quantize.hpp>
#include <core/core.hpp>
#include <spdlog/spdlog.h>

using namespace dip;

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);
  spdlog::info("Algorithm library demo");

  if (argc < 2) {
    spdlog::error("Usage: {} <image_file>", argv[0]);
    return 1;
  }

  std::string filename = argv[1];
  spdlog::info("Loading image: {}", filename);

  auto img = ImageLoader::load_from_file(filename);
  if (!img || img->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  spdlog::info("Original image: {}x{}x{}", img->width(), img->height(),
               img->channels());

  // 测试最近邻插值
  try {
    spdlog::info("Testing nearest neighbor zoom...");
    auto zoomed = algorithms::nearest_neighbor_zoom(*img, 2.0f);
    spdlog::info("Zoomed image: {}x{}x{}", zoomed.width(), zoomed.height(),
                 zoomed.channels());

    if (image_saver::save_binary(zoomed, "zoomed_demo.ppm")) {
      spdlog::info("Saved zoomed image: zoomed_demo.ppm");
    }
  } catch (const std::exception &e) {
    spdlog::error("Zoom failed: {}", e.what());
  }

  // 测试量化
  try {
    spdlog::info("Testing quantization...");
    auto quantized = algorithms::quantize(*img, 16);
    spdlog::info("Quantized image: {}x{}x{}", quantized.width(),
                 quantized.height(), quantized.channels());

    if (image_saver::save_binary(quantized, "quantized_demo.ppm")) {
      spdlog::info("Saved quantized image: quantized_demo.ppm");
    }
  } catch (const std::exception &e) {
    spdlog::error("Quantization failed: {}", e.what());
  }

  // 测试下采样
  try {
    spdlog::info("Testing downsampling...");
    auto downsampled = algorithms::downsample(*img, 4);
    spdlog::info("Downsampled image: {}x{}x{}", downsampled.width(),
                 downsampled.height(), downsampled.channels());

    if (image_saver::save_binary(downsampled, "downsampled_demo.ppm")) {
      spdlog::info("Saved downsampled image: downsampled_demo.ppm");
    }
  } catch (const std::exception &e) {
    spdlog::error("Downsampling failed: {}", e.what());
  }

  spdlog::info("Demo completed!");
  return 0;
}
