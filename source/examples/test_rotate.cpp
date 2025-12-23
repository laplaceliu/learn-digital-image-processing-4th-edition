#include <algorithms/rotate.hpp>
#include <cmath>
#include <core/core.hpp>
#include <spdlog/spdlog.h>

using namespace dip;

int main(int argc, char *argv[]) {
  spdlog::set_level(spdlog::level::info);

  std::string filename = argc > 1 ? argv[1] : "test.png";
  auto img = ImageLoader::load_from_file(filename);

  if (!img || img->empty()) {
    spdlog::error("Failed to load image: {}", filename);
    return 1;
  }

  // 测试不同角度的旋转
  std::vector<double> angles = {0.0, M_PI / 6, M_PI / 4, M_PI / 2, M_PI};

  for (double angle : angles) {
    try {
      auto rotated = dip::algorithms::rotate(*img, angle);

      double degrees = angle * 180.0 / M_PI;
      std::string output_name =
          "rotate_" + std::to_string(static_cast<int>(degrees)) + "deg.ppm";

      // 保存结果
      if (image_saver::save_binary(rotated, output_name)) {
        spdlog::info("Rotated {:.1f} degrees: saved as {}", degrees,
                     output_name);
      } else {
        spdlog::error("Failed to save rotated image: {}", output_name);
      }

    } catch (const std::exception &e) {
      spdlog::error("Error rotating image by {:.1f} degrees: {}",
                    angle * 180.0 / M_PI, e.what());
    }
  }

  spdlog::info("Rotation test completed!");
  return 0;
}
