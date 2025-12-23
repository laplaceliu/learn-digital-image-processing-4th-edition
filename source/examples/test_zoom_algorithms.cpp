#include <algorithms/bilinear_zoom.hpp>
#include <algorithms/nearest_neighbor_zoom.hpp>
#include <chrono>
#include <core/core.hpp>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace dip;

void compare_zoom_methods(const Image &img, float scale) {
  spdlog::info("Comparing zoom methods with scale factor: {:.2f}", scale);

  // 最近邻插值
  auto start_nn = std::chrono::high_resolution_clock::now();
  auto zoomed_nn = algorithms::nearest_neighbor_zoom(img, scale);
  auto end_nn = std::chrono::high_resolution_clock::now();
  auto time_nn =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_nn - start_nn);

  // 双线性插值
  auto start_bl = std::chrono::high_resolution_clock::now();
  auto zoomed_bl = algorithms::bilinear_zoom(img, scale);
  auto end_bl = std::chrono::high_resolution_clock::now();
  auto time_bl =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_bl - start_bl);

  spdlog::info("Nearest Neighbor Zoom: {}x{}x{}, Time: {}ms", zoomed_nn.width(),
               zoomed_nn.height(), zoomed_nn.channels(), time_nn.count());
  spdlog::info("Bilinear Zoom:      {}x{}x{}, Time: {}ms", zoomed_bl.width(),
               zoomed_bl.height(), zoomed_bl.channels(), time_bl.count());

  // 保存结果用于对比
  std::string nn_output = "nearest_neighbor_" +
                          std::to_string(static_cast<int>(scale * 10)) +
                          "x.ppm";
  std::string bl_output =
      "bilinear_" + std::to_string(static_cast<int>(scale * 10)) + "x.ppm";

  if (image_saver::save_binary(zoomed_nn, nn_output)) {
    spdlog::info("Saved nearest neighbor result: {}", nn_output);
  }

  if (image_saver::save_binary(zoomed_bl, bl_output)) {
    spdlog::info("Saved bilinear result: {}", bl_output);
  }

  spdlog::info("Compare the two images to see the quality difference!");
  spdlog::info("Nearest neighbor: sharper but more pixelated");
  spdlog::info("Bilinear: smoother but slightly more blurry");
}

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);
  spdlog::info("Zoom Algorithm Comparison Test");

  std::string filename = argc > 1 ? argv[1] : "test.png";
  float scale = argc > 2 ? std::atof(argv[2]) : 2.5f;

  spdlog::info("Loading image: {}, scale: {:.2f}", filename, scale);

  auto img = ImageLoader::load_from_file(filename);
  if (!img || img->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  spdlog::info("Original image: {}x{}x{}", img->width(), img->height(),
               img->channels());

  try {
    compare_zoom_methods(*img, scale);
  } catch (const std::exception &e) {
    spdlog::error("Error: {}", e.what());
    return 1;
  }

  spdlog::info("Comparison completed!");
  return 0;
}
