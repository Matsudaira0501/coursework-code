import numpy as np
import unittest

from sklearn.datasets import load_iris, load_digits
from sklearn.model_selection import train_test_split

# ===== ColabないしJupyter上で実行する際は消す ===== #

from neural_network import MyMLP

# ===== ここまで ===== #

# global test settings
RAND_SEED = 1289
SAMPLE_NUM_S = 10
SAMPLE_NUM_M = 100
SAMPLE_NUM_L = 1000


def extend(X) -> "np.ndarray[np.ndarray[float]]":
    """与えられたd次元特徴ベクトルの配列をd+1次元の拡張特徴ベクトルの配列へ変換し返す．"""
    one = np.ones((len(X), 1))
    return np.hstack((one, X))


def to_one_hot(y, class_num) -> "np.ndarray[np.ndarray[int]]":
    """与えられた教師信号の配列をone-hot表現に変換し，返す．"""
    return np.identity(class_num)[y]


def arr2str(arr):
    return "\n" + np.array2string(arr, separator=", ")


class NeuralNetworkTestCase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(NeuralNetworkTestCase, self).__init__(*args, **kwargs)

        self.RHO = 1
        self.M = 4
        self.iris = load_iris()
        self.n_iris = len(self.iris.data)
        self.d_iris = len(self.iris.data[0])
        self.c_iris = len(np.unique(self.iris.target))

    def test_1_g(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        X = extend(self.iris.data[indexes])
        W_AB = rng.standard_normal((self.d_iris + 1, self.M))
        W_BC = rng.standard_normal((self.M, self.c_iris))

        # prepare expected results
        G_B_expected = np.array(
            [
                [9.99972154e-01, 7.47779400e-06, 9.99510724e-01, 9.99994949e-01],
                [9.98893217e-01, 3.34380715e-05, 9.97723297e-01, 9.99711683e-01],
                [9.98969773e-01, 4.72310283e-05, 9.97730237e-01, 9.99736369e-01],
                [9.99981989e-01, 2.64819174e-05, 9.99590653e-01, 9.99995934e-01],
                [9.99988262e-01, 1.13161856e-05, 9.99679468e-01, 9.99997548e-01],
                [9.98112216e-01, 9.07657710e-05, 9.95773134e-01, 9.99590289e-01],
                [9.99985649e-01, 1.70002556e-05, 9.99681841e-01, 9.99996281e-01],
                [9.99967091e-01, 9.41615795e-06, 9.99439670e-01, 9.99994088e-01],
                [9.99847216e-01, 3.58719026e-05, 9.98113000e-01, 9.99978431e-01],
                [9.98910597e-01, 3.57804480e-05, 9.96364627e-01, 9.99794773e-01],
            ]
        )
        G_C_expected = np.array(
            [
                [0.39197227, 0.8940675, 0.22778755],
                [0.39217285, 0.89401733, 0.22794967],
                [0.39216268, 0.89402513, 0.22793066],
                [0.39197634, 0.89406563, 0.22778546],
                [0.39197145, 0.89406374, 0.22779008],
                [0.39233303, 0.89401544, 0.22803098],
                [0.39197355, 0.89406327, 0.22778984],
                [0.39197329, 0.89406917, 0.22778553],
                [0.39199513, 0.89409943, 0.22775464],
                [0.39216786, 0.89407337, 0.22788844],
            ]
        )

        # check the given module computes expected values
        for i, x in enumerate(X):
            g_B = MyMLP.g(x, W_AB)
            g_B_expected = G_B_expected[i]
            try:
                self.assertTrue(np.allclose(g_B, g_B_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("x:", arr2str(x))
                print("W_AB:", arr2str(W_AB))
                print("expected:", arr2str(g_B_expected))
                print("actual:", arr2str(g_B))
                raise

            g_C = MyMLP.g(g_B, W_BC)
            g_C_expected = G_C_expected[i]
            try:
                self.assertTrue(np.allclose(g_C, g_C_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("g_B:", arr2str(g_B))
                print("W_BC:", arr2str(W_BC))
                print("expected:", arr2str(g_C_expected))
                print("actual:", arr2str(g_C))
                raise

    def test_2_compute_epsilon(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        indexes = rng.integers(0, self.n_iris, size=SAMPLE_NUM_S)
        y_one_hot = to_one_hot(self.iris.target[indexes], self.c_iris)
        G = np.array(
            [
                [0.39197227, 0.8940675, 0.22778755],
                [0.39217285, 0.89401733, 0.22794967],
                [0.39216268, 0.89402513, 0.22793066],
                [0.39197634, 0.89406563, 0.22778546],
                [0.39197145, 0.89406374, 0.22779008],
                [0.39233303, 0.89401544, 0.22803098],
                [0.39197355, 0.89406327, 0.22778984],
                [0.39197329, 0.89406917, 0.22778553],
                [0.39199513, 0.89409943, 0.22775464],
                [0.39216786, 0.89407337, 0.22788844],
            ]
        )

        # prepare expected results
        E_expected = np.array(
            [
                [0.09341875, -0.01003295, 0.04006792],
                [-0.14488977, 0.08470845, 0.04011655],
                [-0.14489086, 0.08470369, 0.04011084],
                [0.09342007, 0.08467899, -0.13583195],
                [0.09341849, 0.08468015, -0.13583308],
                [-0.14487256, 0.0847096, 0.04014094],
                [0.09341917, 0.08468043, -0.13583302],
                [0.09341908, -0.01003265, 0.04006731],
                [0.09342614, -0.01002726, 0.04005805],
                [-0.1448903, 0.08467427, 0.04009818],
            ]
        )

        # check the given module computes expected values
        for i, b in enumerate(y_one_hot):
            g = G[i]
            err = g - b
            e = MyMLP.compute_epsilon(err, g)
            e_expected = E_expected[i]
            try:
                self.assertTrue(np.allclose(e, e_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("err:", arr2str(err))
                print("g:", arr2str(g))
                print("expected:", arr2str(e_expected))
                print("actual:", arr2str(e))
                raise

    def test_3_update_W(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        model = MyMLP(rho=self.RHO)
        W = rng.standard_normal((self.M, self.c_iris))
        X = np.array(
            [
                [9.99972154e-01, 7.47779400e-06, 9.99510724e-01, 9.99994949e-01],
                [9.98893217e-01, 3.34380715e-05, 9.97723297e-01, 9.99711683e-01],
                [9.98969773e-01, 4.72310283e-05, 9.97730237e-01, 9.99736369e-01],
                [9.99981989e-01, 2.64819174e-05, 9.99590653e-01, 9.99995934e-01],
                [9.99988262e-01, 1.13161856e-05, 9.99679468e-01, 9.99997548e-01],
                [9.98112216e-01, 9.07657710e-05, 9.95773134e-01, 9.99590289e-01],
                [9.99985649e-01, 1.70002556e-05, 9.99681841e-01, 9.99996281e-01],
                [9.99967091e-01, 9.41615795e-06, 9.99439670e-01, 9.99994088e-01],
                [9.99847216e-01, 3.58719026e-05, 9.98113000e-01, 9.99978431e-01],
                [9.98910597e-01, 3.57804480e-05, 9.96364627e-01, 9.99794773e-01],
            ]
        )
        E = np.array(
            [
                [0.09341875, -0.01003295, 0.04006792],
                [-0.14488977, 0.08470845, 0.04011655],
                [-0.14489086, 0.08470369, 0.04011084],
                [0.09342007, 0.08467899, -0.13583195],
                [0.09341849, 0.08468015, -0.13583308],
                [-0.14487256, 0.0847096, 0.04014094],
                [0.09341917, 0.08468043, -0.13583302],
                [0.09341908, -0.01003265, 0.04006731],
                [0.09342614, -0.01002726, 0.04005805],
                [-0.1448903, 0.08467427, 0.04009818],
            ]
        )

        # prepare expected results
        W_expected_arr = np.array(
            [
                [
                    [2.48239672e-01, 1.47350567e-01, 6.12699988e-01],
                    [9.24904580e-02, 1.05702698e-01, -6.72867954e-01],
                    [-9.49958361e-01, 4.81636458e-01, 8.00595835e-01],
                    [8.65410253e-01, -1.74107805e00, -8.85953539e-04],
                ],
                [
                    [4.86385229e-01, 5.27032002e-02, 6.12694643e-01],
                    [9.24960014e-02, 1.05699790e-01, -6.72868996e-01],
                    [-7.12025420e-01, 3.87092823e-01, 8.00618934e-01],
                    [1.10367653e00, -1.83579497e00, -9.23219639e-04],
                ],
                [
                    [4.86397410e-01, 5.27014704e-02, 6.12697276e-01],
                    [9.24979999e-02, 1.05698622e-01, -6.72869549e-01],
                    [-7.12023327e-01, 3.87096984e-01, 8.00624353e-01],
                    [1.10368119e00, -1.83579230e00, -9.18501461e-04],
                ],
                [
                    [2.48237433e-01, 5.26404315e-02, 7.88596296e-01],
                    [9.24886826e-02, 1.05700380e-01, -6.72864058e-01],
                    [-9.49967148e-01, 3.86964090e-01, 9.76420498e-01],
                    [8.65408841e-01, -1.83578959e00, 1.75013162e-01],
                ],
                [
                    [2.48238427e-01, 5.26387403e-02, 7.88598278e-01],
                    [9.24900994e-02, 1.05701664e-01, -6.72866118e-01],
                    [-9.49973865e-01, 3.86955409e-01, 9.76433692e-01],
                    [8.65410270e-01, -1.83579089e00, 1.75014511e-01],
                ],
                [
                    [4.86254892e-01, 5.27682098e-02, 6.12701630e-01],
                    [9.25043060e-02, 1.05694934e-01, -6.72871298e-01],
                    [-7.12325116e-01, 3.87256873e-01, 8.00672881e-01],
                    [1.10364174e00, -1.83578584e00, -9.42729737e-04],
                ],
                [
                    [2.48237991e-01, 5.26386816e-02, 7.88597863e-01],
                    [9.24895684e-02, 1.05701183e-01, -6.72865346e-01],
                    [-9.49974767e-01, 3.86954929e-01, 9.76433954e-01],
                    [8.65409708e-01, -1.83579106e00, 1.75014279e-01],
                ],
                [
                    [2.48239815e-01, 1.47350216e-01, 6.12700801e-01],
                    [9.24902769e-02, 1.05702717e-01, -6.72868032e-01],
                    [-9.49952053e-01, 4.81635445e-01, 8.00599291e-01],
                    [8.65410003e-01, -1.74107835e00, -8.85309044e-04],
                ],
                [
                    [2.48243955e-01, 1.47343624e-01, 6.12714863e-01],
                    [9.24878052e-02, 1.05702982e-01, -6.72869092e-01],
                    [-9.49835164e-01, 4.81616755e-01, 8.00661690e-01],
                    [8.65404406e-01, -1.74108390e00, -8.75421910e-04],
                ],
                [
                    [4.86388277e-01, 5.27358708e-02, 6.12712295e-01],
                    [9.24963408e-02, 1.05699593e-01, -6.72869089e-01],
                    [-7.12221749e-01, 3.87241969e-01, 8.00691742e-01],
                    [1.10368910e00, -1.83576784e00, -9.08186693e-04],
                ],
            ]
        )

        # check the given module computes expected values
        for i, x in enumerate(X):
            e = E[i]
            W_upd = model.update_W(W, x, e)
            W_upd_expected = W_expected_arr[i]
            try:
                self.assertTrue(np.allclose(W_upd, W_upd_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("x:", arr2str(x))
                print("e:", arr2str(e))
                print("W:", arr2str(W))
                print("expected:", arr2str(W_upd_expected))
                print("actual:", arr2str(W_upd))
                raise

    def test_4_iris(self):
        # internal constants
        TEST_RATIO = 0.2
        LOSS_THRESHOLD = 0.03

        # prepare a target data
        X_train, X_test, y_train, _ = train_test_split(
            self.iris.data,
            self.iris.target,
            test_size=TEST_RATIO,
            random_state=RAND_SEED,
        )
        X_train /= X_train.max()
        X_test /= X_test.max()
        model = MyMLP(
            rho=self.RHO,
            hidden_unit_size=self.M,
            loss_threshold=LOSS_THRESHOLD,
            random_seed=RAND_SEED,
        )
        model.fit(X_train, y_train)
        y_predicted = model.predict(X_test)

        # prepare expected results
        y_expected = np.array(
            [
                1,
                0,
                2,
                2,
                2,
                1,
                2,
                2,
                0,
                2,
                2,
                1,
                2,
                2,
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

        # check the given module computes expected values
        try:
            self.assertTrue(np.allclose(y_expected, y_predicted))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Iris Dataset")
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("MyMLP:")
            print("  hidden_unit_size:", self.M)
            print("  loss_threshold:", LOSS_THRESHOLD)
            print("  random_state:", RAND_SEED)
            print("expected:", arr2str(y_expected))
            print("actual:", arr2str(y_predicted))
            raise

    def test_5_digits(self):
        # internal constants
        C_DIGITS = 10
        TEST_RATIO = 0.02
        M = 20
        LOSS_THRESHOLD = 0.01

        # prepare a target data
        X_digits, y_digits = load_digits(n_class=C_DIGITS, return_X_y=True)
        X_train, X_test, y_train, _ = train_test_split(
            X_digits, y_digits, test_size=TEST_RATIO, random_state=RAND_SEED
        )
        X_train /= X_train.max()
        X_test /= X_test.max()
        model = MyMLP(
            rho=self.RHO,
            hidden_unit_size=M,
            loss_threshold=LOSS_THRESHOLD,
            random_seed=RAND_SEED,
        )
        model.fit(X_train, y_train)
        y_predicted = model.predict(X_test)

        # prepare expected results
        y_expected = np.array(
            [
                2,
                0,
                4,
                1,
                8,
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
                1,
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
            print("MyMLP:")
            print("  hidden_unit_size:", M)
            print("  loss_threshold:", LOSS_THRESHOLD)
            print("  random_state:", RAND_SEED)
            print("expected:", arr2str(y_expected))
            print("actual:", arr2str(y_predicted))
            raise


if __name__ == "__main__":
    unittest.main(argv=["dummy argument"], verbosity=2, exit=False)
