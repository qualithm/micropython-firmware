"""What this firmware image carries, frozen into it beside the device SDK.

The CLI reads SDK_VERSION from a board to tell whether its firmware already holds the device SDK,
so a deploy doesn't copy one to /lib that the frozen copy would shadow.
"""

# The device-sdk-py release frozen into the image: vendor/device-sdk-py/VERSION.
SDK_VERSION = "0.9.3"


def sdk_matches(version: str) -> bool:
    """Say whether the frozen device SDK is the given release.

    Args:
        version: a device-sdk-py release, such as "0.9.3".

    Returns:
        True when the image's frozen SDK is that release.
    """
    return version.strip() == SDK_VERSION
