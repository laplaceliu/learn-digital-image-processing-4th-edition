#include <algorithms/nearest_neighbor_zoom.hpp>
#include <core/core.hpp>
#include <spdlog/spdlog.h>

using namespace dip;

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);
  spdlog::info("Nearest Neighbor Zoom Test");

  std::string filename = argc > 1 ? argv[1] : "test.png";
  float scale = argc > 2 ? std::atof(argv[2]) : 2.0f;

  spdlog::info("Loading image: {}, scale: {}", filename, scale);

  auto img = ImageLoader::load_from_file(filename);
  if (!img || img->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  spdlog::info("Original image: {}x{}x{}", img->width(), img->height(),
               img->channels());

  try {
    auto result = algorithms::nearest_neighbor_zoom(*img, scale);
    spdlog::info("Zoomed image: {}x{}x{}", result.width(), result.height(),
                 result.channels());

    std::string output =
        "zoomed_" + std::to_string(static_cast<int>(scale * 10)) + "x.ppm";
    if (image_saver::save_binary(result, output)) {
      spdlog::info("Saved result: {}", output);
    }
  } catch (const std::exception &e) {
    spdlog::error("Error: {}", e.what());
    return 1;
  }

  return 0;
}
