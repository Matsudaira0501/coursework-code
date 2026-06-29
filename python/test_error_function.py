from operator import index
import numpy as np
import unittest

from sklearn.datasets import load_iris, load_digits
from sklearn.model_selection import train_test_split

# ===== ColabないしJupyter上で実行する際は消す ===== #

from error_function import extend
from error_function import to_one_hot
from error_function import pseudo_inv
from error_function import MyLinear

# ===== ここまで ===== #

# global test settings
RAND_SEED = 1289
SAMPLE_NUM_S = 10
SAMPLE_NUM_M = 100
SAMPLE_NUM_L = 1000


def arr2str(arr):
    return "\n" + np.array2string(arr, separator=", ")


class MyLinearTestCase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(MyLinearTestCase, self).__init__(*args, **kwargs)

        self.iris = load_iris()
        self.n_iris = len(self.iris.data)
        self.d_iris = len(self.iris.data[0])
        self.c_iris = len(np.unique(self.iris.target))

    def test_1_to_one_hot(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        y = self.iris.target[indexes]

        # prepare expected results
        y_one_hot_expected = np.array(
            [
                [0.0, 1.0, 0.0],
                [1.0, 0.0, 0.0],
                [1.0, 0.0, 0.0],
                [0.0, 0.0, 1.0],
                [0.0, 0.0, 1.0],
                [1.0, 0.0, 0.0],
                [0.0, 0.0, 1.0],
                [0.0, 1.0, 0.0],
                [0.0, 1.0, 0.0],
                [1.0, 0.0, 0.0],
            ]
        )

        # check the given module computes expected values
        try:
            y_one_hot = to_one_hot(y, self.c_iris)
            self.assertTrue(np.allclose(y_one_hot, y_one_hot_expected))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("y:", arr2str(y))
            print("expected:", arr2str(y_one_hot_expected))
            print("actual:", arr2str(y_one_hot))
            raise

    def test_2_pseudo_inv(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        X = self.iris.data[indexes]
        X_ext = extend(X)

        # prepare expected results
        X_pinv_expected = np.linalg.pinv(X_ext)

        # check the given module computes expected values
        try:
            X_pinv = pseudo_inv(X_ext, e=0)
            self.assertTrue(np.allclose(X_pinv, X_pinv_expected))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("X:", arr2str(X))
            print("expected:", arr2str(X_pinv_expected))
            print("actual:", arr2str(X_pinv))
            raise

    def test_3_compute_w(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        X = self.iris.data[indexes]
        X_ext = extend(X)
        y = self.iris.target[indexes]
        y_one_hot = to_one_hot(y, self.c_iris)

        # prepare expected results
        W = np.array(
            [
                [2.15994592, -0.0982866, -0.07329985, -0.47011444, 0.56883043],
                [-3.45090904, 0.22692466, 0.48803004, 1.07813045, -2.41969736],
                [2.2909628, -0.12863805, -0.41473012, -0.60801597, 1.85086683],
            ]
        )

        # check the given module computes expected values
        for i in range(self.c_iris):
            try:
                y_one_hot_i = y_one_hot[:, i]
                w = MyLinear.compute_w(X_ext, y_one_hot_i)
                self.assertTrue(np.allclose(w, W[i]))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("X:", arr2str(X))
                print("y_one_hot_i:", arr2str(y_one_hot_i))
                print("expected:", arr2str(W[i]))
                print("actual:", arr2str(w))
                raise

    def test_4_iris(self):
        # internal constants
        TEST_RATIO = 0.2

        # prepare a target data
        X_train, X_test, y_train, _ = train_test_split(
            self.iris.data,
            self.iris.target,
            test_size=TEST_RATIO,
            random_state=RAND_SEED,
        )
        model = MyLinear()
        model.fit(X_train, y_train)
        y_predicted = model.predict(X_test)

        # prepare expected results
        y_expected = np.array(
            [
                1,
                0,
                2,
                1,
                2,
                1,
                2,
                2,
                0,
                2,
                2,
                1,
                2,
                1,
                1,
                1,
                0,
                2,
                2,
                1,
                0,
                2,
                0,
                0,
                1,
                2,
                0,
                2,
                0,
                2,
            ]
        )

        # check the given module computes expected values
        try:
            self.assertTrue(np.allclose(y_expected, y_predicted))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Iris Dataset")
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("expected:", arr2str(y_expected))
            print("actual:", arr2str(y_predicted))
            raise

    def test_5_digits(self):
        # internal constants
        C_DIGITS = 10
        TEST_RATIO = 0.02

        # prepare a target data
        X_digits, y_digits = load_digits(n_class=C_DIGITS, return_X_y=True)
        X_train, X_test, y_train, _ = train_test_split(
            X_digits, y_digits, test_size=TEST_RATIO, random_state=RAND_SEED
        )
        model = MyLinear()
        model.fit(X_train, y_train)
        y_predicted = model.predict(X_test)

        # prepare expected results
        y_expected = np.array(
            [
                2,
                0,
                7,
                1,
                0,
                9,
                5,
                6,
                9,
                8,
                3,
                8,
                0,
                2,
                1,
                6,
                3,
                0,
                3,
                9,
                8,
                5,
                8,
                8,
                4,
                4,
                7,
                4,
                4,
                0,
                6,
                1,
                8,
                6,
                6,
                8,
            ]
        )

        # check the given model computes expected values
        try:
            self.assertTrue(np.allclose(y_expected, y_predicted))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Digits Dataset")
            print("  n_class:", C_DIGITS)
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("expected:", arr2str(y_expected))
            print("actual:", arr2str(y_predicted))
            raise


if __name__ == "__main__":
    unittest.main(argv=["dummy argument"], verbosity=2, exit=False)
