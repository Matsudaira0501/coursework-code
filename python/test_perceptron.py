import numpy as np
import unittest

from sklearn.datasets import load_iris, load_digits
from sklearn.model_selection import train_test_split

# ===== ColabないしJupyter上で実行する際は消す ===== #

from perceptron import extend
from perceptron import MyPerceptron

# ===== ここまで ===== #

# global test settings
RAND_SEED = 1289
TEST_RATIO = 0.2
SAMPLE_NUM_S = 10
SAMPLE_NUM_M = 100
SAMPLE_NUM_L = 1000


class MyPerceptronTestCase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(MyPerceptronTestCase, self).__init__(*args, **kwargs)

        self.iris = load_iris()
        self.n_iris = len(self.iris.data)
        self.d_iris = len(self.iris.data[0])
        self.c_iris = len(np.unique(self.iris.target))

    def _assert_equal_ndarray(self, expected, actual):
        if actual.ndim == 1:
            for i in range(len(actual)):
                self.assertAlmostEqual(expected[i], actual[i])
        else:
            for i in range(len(actual)):
                self._assert_equal_ndarray(expected[i], actual[i])

    def _get_iris_dataset(self):
        return train_test_split(
            self.iris.data,
            self.iris.target,
            test_size=TEST_RATIO,
            random_state=RAND_SEED,
        )

    def test_1_g(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target model and feature vectors
        model = MyPerceptron(W=rng.random((self.c_iris, self.d_iris + 1)))
        X, _, _, _ = self._get_iris_dataset()
        X_ext = extend(X)

        # prepare expected results
        expected_g_outs = np.array(
            [
                [8.27492359, 8.66968388, 7.98062729],
                [8.04011919, 8.44780389, 7.71811751],
                [9.6279227, 9.99345423, 9.06012117],
                [10.2265214, 10.58853519, 9.64486781],
                [5.29797906, 5.69023207, 5.52327189],
                [10.60733952, 10.99631273, 9.84647686],
                [8.76001115, 9.25423013, 8.30570602],
                [8.08241147, 8.46654934, 7.76951727],
                [7.49073971, 7.9286246, 7.2340567],
                [9.96447529, 10.34813332, 9.30305836],
            ]
        )

        # check the given model computes expected values
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        for i in range(SAMPLE_NUM_S):
            x = X_ext[indexes[i]]
            g_out = model.g(x)
            expected_g = expected_g_outs[i]
            try:
                self._assert_equal_ndarray(expected_g, g_out)
            except:
                print("\n\nA unittest was failed with the following values:")
                print("W:\n" + str(model.W))
                print("x:", x)
                print("expected:", expected_g)
                print("actual:", g_out)
                raise

    def test_2_select_max_class(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare dummy outputs and expected results
        g_outs = rng.random((SAMPLE_NUM_S, self.c_iris)) - 0.5
        expected_y = np.array([1, 2, 0, 2, 1, 1, 1, 1, 1, 0])

        # check the given model computes expected values
        for i in range(SAMPLE_NUM_S):
            g_out = g_outs[i]
            predicted_class = MyPerceptron.select_max_class(g_out)
            expected = expected_y[i]
            try:
                self.assertEqual(expected, predicted_class)
            except:
                print("\n\nA unittest was failed with the following values:")
                print("g_out:", g_out)
                print("expected:", expected)
                print("actual:", predicted_class)
                raise

    def test_3_update_W(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target model and feature vectors
        model = MyPerceptron(W=rng.random((self.c_iris, self.d_iris + 1)))
        X, _, y, _ = self._get_iris_dataset()
        X_ext = extend(X)

        # prepare expected results
        expected_Ws = np.array(
            [
                [
                    [1.14724835, 7.28421835, 3.25796072, 5.00500301, 1.50712661],
                    [-0.07539113, -5.61775142, -3.13456973, -3.95756294, -1.44110634],
                    [0.61654304, 0.88144036, 0.08622171, 0.29512217, 0.07927115],
                ],
                [
                    [2.14724835, 13.48421835, 5.45796072, 9.50500301, 3.00712661],
                    [-1.07539113, -11.81775142, -5.33456973, -8.45756294, -2.94110634],
                    [0.61654304, 0.88144036, 0.08622171, 0.29512217, 0.07927115],
                ],
                [
                    [1.14724835, 6.28421835, 2.45796072, 3.70500301, 1.40712661],
                    [-1.07539113, -11.81775142, -5.33456973, -8.45756294, -2.94110634],
                    [1.61654304, 8.08144036, 3.08622171, 6.09512217, 1.67927115],
                ],
                [
                    [0.14724835, -1.41578165, -0.54203928, -2.39499699, -0.89287339],
                    [-1.07539113, -11.81775142, -5.33456973, -8.45756294, -2.94110634],
                    [2.61654304, 15.78144036, 6.08622171, 12.19512217, 3.97927115],
                ],
                [
                    [1.14724835, 3.28421835, 2.65796072, -0.79499699, -0.69287339],
                    [-2.07539113, -16.51775142, -8.53456973, -10.05756294, -3.14110634],
                    [2.61654304, 15.78144036, 6.08622171, 12.19512217, 3.97927115],
                ],
                [
                    [0.14724835, -4.41578165, 0.05796072, -7.69499699, -2.99287339],
                    [-2.07539113, -16.51775142, -8.53456973, -10.05756294, -3.14110634],
                    [3.61654304, 23.48144036, 8.68622171, 19.09512217, 6.27927115],
                ],
                [
                    [-0.85275165, -10.71578165, -3.34203928, -13.29499699, -5.39287339],
                    [-2.07539113, -16.51775142, -8.53456973, -10.05756294, -3.14110634],
                    [4.61654304, 29.78144036, 12.08622171, 24.69512217, 8.67927115],
                ],
                [
                    [-0.85275165, -10.71578165, -3.34203928, -13.29499699, -5.39287339],
                    [-1.07539113, -10.21775142, -6.23456973, -5.65756294, -1.84110634],
                    [3.61654304, 23.48144036, 9.78622171, 20.29512217, 7.37927115],
                ],
                [
                    [-0.85275165, -10.71578165, -3.34203928, -13.29499699, -5.39287339],
                    [-0.07539113, -4.51775142, -3.23456973, -1.45756294, -0.64110634],
                    [2.61654304, 17.78144036, 6.78622171, 16.09512217, 6.17927115],
                ],
                [
                    [0.14724835, -3.41578165, -0.44203928, -6.99499699, -3.59287339],
                    [-1.07539113, -11.81775142, -6.13456973, -7.75756294, -2.44110634],
                    [2.61654304, 17.78144036, 6.78622171, 16.09512217, 6.17927115],
                ],
            ]
        )

        # check the given model computes expected values
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        for i in range(SAMPLE_NUM_S):
            prev_W = model.W

            x = X_ext[indexes[i]]
            correct_class = y[i]
            predicted_class = (correct_class + 1) % self.c_iris
            model.update_W(x, predicted_class, correct_class)

            expected_W = expected_Ws[i]
            try:
                self._assert_equal_ndarray(expected_W, model.W)
            except:
                print("\n\nA unittest was failed with the following values:")
                print("x:", x)
                print("W:\n" + str(prev_W))
                print("expected:\n" + str(expected_W))
                print("actual:\n" + str(model.W))
                raise

    def test_4_binary_digits(self):
        # load Digits dataset with binary classes
        c_digits = 2
        X_digits, y_digits = load_digits(n_class=c_digits, return_X_y=True)
        d_digits = len(X_digits[0])

        # prepare a target model and Digits dataset
        model = MyPerceptron(rho=1.0, W=np.ones((c_digits, d_digits + 1)), max_epoch=1000)
        X_train, X_test, y_train, y_test = train_test_split(
            X_digits, y_digits, test_size=TEST_RATIO, random_state=RAND_SEED
        )

        # fit and predict Iris dataset
        model.fit(X_train, y_train)
        y_predicted = model.predict(X_test)

        # check the given model computes expected values
        try:
            self.assertTrue(np.all(y_predicted == y_test))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Digits Dataset")
            print("  n_class:", c_digits)
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("expected:\n" + str(y_test))
            print("actual:\n" + str(y_predicted))
            raise

    def test_5_iris(self):
        # prepare a target model and Iris dataset
        model = MyPerceptron(rho=1.0, W=np.ones((self.c_iris, self.d_iris + 1)), max_epoch=1000)
        X_train, X_test, y_train, _ = self._get_iris_dataset()

        # prepare expected results
        y_expected = np.array(
            [
                1,
                0,
                2,
                1,
                1,
                1,
                2,
                1,
                0,
                2,
                1,
                1,
                2,
                1,
                1,
                1,
                0,
                1,
                2,
                2,
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

        # fit and predict Iris dataset
        model.fit(X_train, y_train)
        y_predicted = model.predict(X_test)

        # check the given model computes expected values
        try:
            self._assert_equal_ndarray(y_expected, y_predicted)
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Iris Dataset")
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("expected:\n" + str(y_expected))
            print("actual:\n" + str(y_predicted))
            raise


if __name__ == "__main__":
    unittest.main(argv=["dummy argument"], verbosity=2, exit=False)
