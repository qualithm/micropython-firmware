# What the image freezes: the port's own modules, this repo's modules/, and the
# device SDK release vendored in vendor/device-sdk-py/.
include("$(PORT_DIR)/boards/manifest.py")
freeze("$(BOARD_DIR)/../../modules")
package("qualithm_device", base_path="$(BOARD_DIR)/../../vendor/device-sdk-py/src")
