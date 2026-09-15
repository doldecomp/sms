import contextlib
import importlib.util
import io
from pathlib import Path
import unittest
from unittest.mock import patch


def load(name):
    spec = importlib.util.spec_from_file_location(
        name, Path(__file__).with_name(name + ".py"))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


symbols = load("validate-symbol-order")
driver = load("check-changed-symbol-order")


class SymbolErrorsTest(unittest.TestCase):
    def errors(self, objects, unused=()):
        syms = [symbols.MapSym(n, 4, n in unused) for n in "abc"]
        return symbols.validation_errors(syms, objects, lambda n: "global")

    def test_inherited_errors_are_not_new(self):
        objects = [("b", 4, "global"), ("a", 4, "global")]
        errors = self.errors(objects)
        self.assertEqual(errors, {("missing", "c"), ("order", "a", "b")})
        self.assertEqual(errors - self.errors(objects), set())

    def test_new_missing_symbol_cannot_replace_old_one(self):
        before = self.errors([("a", 4, "global"), ("b", 4, "global")])
        after = self.errors([("a", 4, "global"), ("c", 4, "global")])
        self.assertEqual(after - before, {("missing", "b")})

    def test_new_inversion_in_already_disordered_unit(self):
        before = self.errors([(n, 4, "global") for n in "bac"])
        after = self.errors([(n, 4, "global") for n in "bca"])
        self.assertEqual(after - before, {("order", "a", "c")})

    def test_linkage_drift_is_new(self):
        before = self.errors([(n, 4, "global") for n in "abc"])
        after = self.errors([("a", 4, "weak"), ("b", 4, "global"),
                             ("c", 4, "global")])
        self.assertEqual(after - before, {("binding", "a", "global", "weak")})

    def test_unused_linkage_and_size_remain_nonblocking(self):
        objects = [("a", 100, "weak"), ("b", 4, "global"), ("c", 4, "global")]
        self.assertEqual(self.errors(objects, unused="a"), set())


class DriverTest(unittest.TestCase):
    def test_unavailable_validator_is_failure(self):
        with patch.object(driver, "source_to_unit", return_value={
            "src/a.cpp": ("mario/a", "build/a.o"),
        }), patch.object(driver.os.path, "exists", return_value=True), \
                patch.object(driver.subprocess, "run") as run, \
                contextlib.redirect_stdout(io.StringIO()):
            run.return_value.returncode = 2
            self.assertEqual(driver.main(["src/a.cpp"]), 1)

    def test_missing_current_object_is_failure(self):
        with patch.object(driver, "source_to_unit", return_value={
            "src/a.cpp": ("mario/a", "build/a.o"),
        }), patch.object(driver.os.path, "exists", return_value=False), \
                contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(driver.main(["src/a.cpp"]), 1)


if __name__ == "__main__":
    unittest.main()
