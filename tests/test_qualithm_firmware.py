import unittest

import qualithm_firmware


class TestQualithmFirmware(unittest.TestCase):
    def test_sdk_version_is_the_vendored_release(self):
        with open("vendor/device-sdk-py/VERSION") as f:
            self.assertEqual(qualithm_firmware.SDK_VERSION, f.read().strip())

    def test_sdk_matches_its_own_version(self):
        self.assertTrue(qualithm_firmware.sdk_matches(qualithm_firmware.SDK_VERSION))
        self.assertTrue(qualithm_firmware.sdk_matches(" 0.9.2\n"))

    def test_sdk_matches_rejects_another_release(self):
        self.assertFalse(qualithm_firmware.sdk_matches("0.9.1"))


if __name__ == "__main__":
    unittest.main()
