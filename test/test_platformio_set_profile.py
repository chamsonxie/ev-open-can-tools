import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class PlatformioSetProfileTest(unittest.TestCase):
    def test_missing_optional_define_is_added(self) -> None:
        source = ROOT / "platformio_profile.example.h"
        with tempfile.TemporaryDirectory() as temp_dir:
            profile = Path(temp_dir) / "platformio_profile.h"
            profile.write_text(
                "\n".join(
                    line
                    for line in source.read_text(encoding="utf-8").splitlines()
                    if "DASH_RGB_STATUS_LED" not in line
                )
                + "\n",
                encoding="utf-8",
            )

            subprocess.run(
                [
                    "python3",
                    str(ROOT / "scripts" / "platformio_set_profile.py"),
                    "--driver",
                    "DRIVER_TWAI",
                    "--vehicle",
                    "HW3",
                    "--enable",
                    "DASH_RGB_STATUS_LED",
                    "--file",
                    str(profile),
                ],
                cwd=ROOT,
                check=True,
                capture_output=True,
                text=True,
            )

            self.assertIn(
                "#define DASH_RGB_STATUS_LED",
                profile.read_text(encoding="utf-8"),
            )

    def test_missing_profile_points_to_example_file(self) -> None:
        with tempfile.TemporaryDirectory() as temp_dir:
            missing_profile = Path(temp_dir) / "platformio_profile.h"

            result = subprocess.run(
                [
                    "python3",
                    str(ROOT / "scripts" / "platformio_set_profile.py"),
                    "--driver",
                    "DRIVER_TWAI",
                    "--vehicle",
                    "HW3",
                    "--file",
                    str(missing_profile),
                ],
                cwd=ROOT,
                capture_output=True,
                text=True,
            )

            self.assertNotEqual(0, result.returncode)
            self.assertIn("platformio_profile.example.h", result.stderr)


if __name__ == "__main__":
    unittest.main()
