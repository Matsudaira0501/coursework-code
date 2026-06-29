import numpy as np
import random
import unittest

from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier

# ===== ColabないしJupyter上で実行する際は消す ===== #

from knn import dist
from knn import get_nearest_pos
from knn import MyKNN

# ===== ここまで ===== #

# global test settings
RAND_SEED = 1289
SAMPLE_NUM = 100
SPACE_MIN = -100
SPACE_MAX = 101
MAX_DIM_SIZE = 128
CLASS_NUM = 3


class MyKNNTestCase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(MyKNNTestCase, self).__init__(*args, **kwargs)

        iris = load_iris()
        self.X_train, self.X_test, self.y_train, _ = train_test_split(
            iris.data,
            iris.target,
            test_size=0.2,
            random_state=RAND_SEED,
        )

    def __test_dist(self, x_0, x_1, x_2):
        """Check imported `dist` function acts as a metric space."""
        try:
            self.assertEqual(0, dist(x_0, x_0))
            self.assertAlmostEqual(dist(x_0, x_1), dist(x_1, x_0))
            try:
                self.assertAlmostEqual(dist(x_0, x_2), dist(x_0, x_1) + dist(x_1, x_2))
            except:
                self.assertLess(dist(x_0, x_2), dist(x_0, x_1) + dist(x_1, x_2))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("x_0: " + str(x_0))
            print("x_1: " + str(x_1))
            print("x_2: " + str(x_2))
            raise

    def test_dist(self):
        self.assertAlmostEqual(0, dist(np.array([0, 0]), np.array([0, 0])))
        self.assertAlmostEqual(5, dist(np.array([0, 0]), np.array([3, 4])))

        for i in range(len(self.X_train) - 2):
            self.__test_dist(self.X_train[i], self.X_train[i + 1], self.X_train[i + 2])

    def test_get_nearest_pos(self):
        x_test = self.X_test[0]
        min_pos = get_nearest_pos(self.X_train, x_test)
        min_dist = dist(self.X_train[min_pos], x_test)
        for x_train in self.X_train:
            cur_dist = dist(x_train, x_test)
            try:
                self.assertAlmostEqual(min_dist, cur_dist)
            except:
                try:
                    self.assertLess(min_dist, cur_dist)
                except:
                    print("\n\nA unittest was failed with the following values:")
                    print("target x_test : " + str(x_test))
                    print("expected x_min: " + str(x_train))
                    print("computed x_min: " + str(self.X_train[min_pos]))
                    raise

    def test_fit(self):
        model = MyKNN()
        model.fit(self.X_train, self.y_train)

        X_param, y_param = model.get_parameters()
        for i in range(len(self.X_train)):
            try:
                for j in range(len(self.X_train[0])):
                    self.assertEqual(X_param[i][j], self.X_train[i][j])
                self.assertEqual(y_param[i], self.y_train[i])
            except:
                print("\n\nA unittest was failed with the following values:")
                print("X_param[i]: " + str(X_param[i]))
                print("X_train[i]: " + str(self.X_train[i]))
                raise

    def test_predict(self):
        models = [MyKNN(), KNeighborsClassifier(n_neighbors=1)]

        for model in models:
            model.fit(self.X_train, self.y_train)

        all_results = [model.predict(self.X_test) for model in models]

        for i in range(len(self.X_test)):
            self.assertEqual(all_results[0][i], all_results[1][i])

    def test_MyKNN(self):
        rng = np.random.default_rng(RAND_SEED)

        models = [MyKNN(), KNeighborsClassifier(n_neighbors=1)]
        for n in range(16, MAX_DIM_SIZE, 16):
            X_train = rng.integers(SPACE_MIN, SPACE_MAX, (SAMPLE_NUM, n))
            y_train = rng.integers(0, CLASS_NUM, SAMPLE_NUM)
            X_test = rng.integers(SPACE_MIN, SPACE_MAX, (SAMPLE_NUM, n))

            for model in models:
                model.fit(X_train, y_train)

            all_results = [model.predict(X_test) for model in models]

            for i in range(SAMPLE_NUM):
                self.assertEqual(all_results[0][i], all_results[1][i])


if __name__ == "__main__":
    unittest.main(argv=["dummy argument"], verbosity=2, exit=False)
