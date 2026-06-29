import numpy as np
import unittest

from sklearn.datasets import load_digits, load_iris
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split

# ===== ColabないしJupyter上で実行する際は消す ===== #

from feature_space import compute_S, compute_w, compute_sorted_eigen, MyKL, MyFisher

# ===== ここまで ===== #

# global test settings
RAND_SEED = 1289
SAMPLE_NUM_S = 10
SAMPLE_NUM_M = 100
SAMPLE_NUM_L = 1000


# prepare datasets for testing
_iris = load_iris()
_n_iris = len(_iris.data)
_d_iris = len(_iris.data[0])
_c_iris = len(np.unique(_iris.target))
_digits = load_digits()
_n_digits = len(_digits.data)
_d_digits = len(_digits.data[0])
_c_digits = len(np.unique(_digits.target))


def arr2str(arr):
    return "\n" + np.array2string(arr, separator=", ")


class FeatureSpaceTestCase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(FeatureSpaceTestCase, self).__init__(*args, **kwargs)

    def test_1_compute_S(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # repeat 10 times with sampled data
        for _ in range(SAMPLE_NUM_S):
            # prepare a target data
            indexes = rng.integers(0, _n_iris, size=SAMPLE_NUM_S)
            X = _iris.data[indexes]
            means = X.mean(axis=0)

            # check the given module computes expected values
            S_actual = compute_S(X, means)
            S_expected = np.cov(X, rowvar=0, bias=1) * SAMPLE_NUM_S
            try:
                self.assertTrue(np.allclose(S_actual, S_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("X:", arr2str(X))
                print("means:", arr2str(means))
                print("expected:", arr2str(S_expected))
                print("actual:", arr2str(S_actual))
                raise

    def test_2_compute_w(self):
        # prepare a target data
        S_w_arr = np.array(
            [
                [
                    [1.652, 0.208, 0.584, -0.008],
                    [0.208, 0.272, 0.266, 0.128],
                    [0.584, 0.266, 0.636, 0.252],
                    [-0.008, 0.128, 0.252, 0.22],
                ],
                [
                    [2.016, 1.396, 1.368, 0.592],
                    [1.396, 1.372, 0.478, 0.294],
                    [1.368, 0.478, 1.824, 0.676],
                    [0.592, 0.294, 0.676, 0.328],
                ],
                [
                    [2.18, 1.002, 0.724, 0.256],
                    [1.002, 0.864, 0.062, 0.132],
                    [0.724, 0.062, 1.052, 0.256],
                    [0.256, 0.132, 0.256, 0.096],
                ],
                [
                    [1.968, 0.832, 0.818, 0.196],
                    [0.832, 0.716, 0.356, 0.204],
                    [0.818, 0.356, 0.82, 0.196],
                    [0.196, 0.204, 0.196, 0.092],
                ],
                [
                    [1.46, 0.83, 0.46, 0.252],
                    [0.83, 1.7, 0.32, 0.322],
                    [0.46, 0.32, 0.34, 0.114],
                    [0.252, 0.322, 0.114, 0.12],
                ],
                [
                    [1.892, 0.854, 1.404, 0.3],
                    [0.854, 0.96, 0.268, -0.024],
                    [1.404, 0.268, 1.848, 0.29],
                    [0.3, -0.024, 0.29, 0.148],
                ],
                [
                    [3.312, 1.324, 2.132, 0.65],
                    [1.324, 0.756, 0.75, 0.262],
                    [2.132, 0.75, 1.484, 0.434],
                    [0.65, 0.262, 0.434, 0.168],
                ],
                [
                    [0.924, 0.596, 0.548, 0.21],
                    [0.596, 0.864, 0.732, 0.33],
                    [0.548, 0.732, 0.9, 0.342],
                    [0.21, 0.33, 0.342, 0.196],
                ],
                [
                    [0.94, 0.196, 0.856, 0.128],
                    [0.196, 0.652, -0.008, 0.206],
                    [0.856, -0.008, 1.032, 0.146],
                    [0.128, 0.206, 0.146, 0.128],
                ],
                [
                    [0.568, 0.678, 0.16, 0.078],
                    [0.678, 0.956, 0.138, -0.032],
                    [0.16, 0.138, 0.068, 0.07],
                    [0.078, -0.032, 0.07, 0.168],
                ],
            ]
        )
        Means_0 = np.array(
            [
                [4.96, 3.54, 1.52, 0.26],
                [5.12, 3.64, 1.56, 0.28],
                [5.04, 3.56, 1.5, 0.28],
                [5.1, 3.52, 1.36, 0.3],
                [5.18, 3.48, 1.46, 0.26],
                [5.0, 3.64, 1.4, 0.22],
                [4.86, 3.42, 1.36, 0.2],
                [4.96, 3.34, 1.56, 0.42],
                [4.76, 3.3, 1.3, 0.2],
                [4.8, 3.38, 1.48, 0.3],
            ]
        )
        Means_1 = np.array(
            [
                [6.4, 3.1, 4.58, 1.48],
                [6.22, 2.7, 4.46, 1.4],
                [5.92, 2.66, 4.16, 1.38],
                [5.88, 3.02, 4.18, 1.46],
                [6.14, 2.84, 4.28, 1.22],
                [6.04, 2.68, 4.18, 1.3],
                [5.9, 2.68, 4.06, 1.22],
                [5.66, 2.74, 4.18, 1.28],
                [5.92, 2.64, 3.84, 1.12],
                [5.68, 2.88, 4.2, 1.28],
            ]
        )

        # prepare expected results
        W_expected = np.array(
            [
                [0.12058574, 0.66605788, -0.72765327, 0.11111572],
                [-0.47298838, 0.65928458, 0.14402954, -0.56646388],
                [-0.08533999, 0.24841463, 0.22002499, -0.93946594],
                [-0.11847071, 0.31987589, 0.17370761, -0.92383428],
                [0.18247543, 0.18883042, -0.6382441, -0.72366447],
                [0.33555872, -0.17787728, -0.21353563, -0.9000903],
                [0.56972226, -0.13110543, -0.69080552, -0.42545933],
                [-0.0952939, 0.71962494, -0.52433748, -0.4451171],
                [-0.13782278, 0.36917967, 0.14966446, -0.9068141],
                [0.50045619, -0.23198809, -0.83186962, 0.06097594],
            ]
        )

        # repeat 10 times with sampled data
        for i, S_w in enumerate(S_w_arr):
            # prepare a target data
            means_0 = Means_0[i]
            means_1 = Means_1[i]

            # check the given module computes expected values
            w_actual = compute_w(S_w, means_0, means_1)
            w_expected = W_expected[i]

            try:
                self.assertTrue(np.allclose(w_actual, w_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("S_w:", arr2str(S_w))
                print("means_0:", arr2str(means_0))
                print("means_1:", arr2str(means_1))
                print("expected:", arr2str(w_expected))
                print("actual:", arr2str(w_actual))
                raise

    def test_3_compute_sorted_eigen(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare expected results
        Eigenvalues_expected = np.array(
            [
                [4.78208594e00, 1.28562335e-01, 8.20135488e-02, 1.83817247e-03],
                [3.58354900e00, 2.65229925e-01, 4.22237812e-02, 3.89729068e-03],
                [4.12569322e00, 2.03288335e-01, 5.50657618e-02, 1.01526794e-02],
                [3.84712885e00, 3.10043008e-01, 6.12567395e-02, 8.47140222e-03],
                [6.20026676e00, 1.04742654e-01, 6.19664032e-02, 2.59241836e-02],
                [1.97935146e00, 5.86328622e-02, 3.30096599e-02, 1.36060220e-02],
                [3.16169850e00, 1.50670529e-01, 3.20716957e-02, 1.70592714e-02],
                [3.41566219e00, 2.61059471e-01, 6.89823113e-02, 3.49602559e-03],
                [4.60597460e00, 1.85809676e-01, 7.20520053e-02, 6.26371540e-03],
                [3.05232481e00, 1.24051080e-01, 6.65000116e-02, 9.42409655e-03],
            ]
        )
        Eigenvectors_expected = np.array(
            [
                [
                    [0.41441787, -0.05940042, 0.82954562, 0.36957202],
                    [-0.11386739, 0.89200803, -0.07150616, 0.43155853],
                    [-0.85016063, -0.27500505, 0.23441791, 0.38294565],
                    [-0.30416927, 0.35378736, 0.50178598, -0.72837242],
                ],
                [
                    [0.40852599, 0.03529577, 0.84222095, 0.35003513],
                    [0.46375568, 0.84882593, -0.19108665, -0.16706617],
                    [0.73095244, -0.51718599, -0.16023914, -0.41539211],
                    [0.28938853, -0.10376612, -0.47798868, 0.82280842],
                ],
                [
                    [0.28484049, -0.19671723, 0.86436011, 0.36475996],
                    [-0.695939, -0.70229475, 0.11166687, -0.0999075],
                    [-0.63297951, 0.57651109, 0.12861681, 0.50042943],
                    [0.18404277, -0.36840112, -0.47314885, 0.77880617],
                ],
                [
                    [0.25440809, -0.23777811, 0.86524728, 0.36067332],
                    [-0.81501635, -0.56900731, 0.04096289, 0.10149418],
                    [-0.46995206, 0.75960724, 0.17414184, 0.41450757],
                    [0.22398646, -0.20661339, -0.46834155, 0.82933537],
                ],
                [
                    [-0.40353101, 0.04387644, -0.84447362, -0.3494308],
                    [0.7800495, -0.31941047, -0.17945134, -0.50724446],
                    [-0.43126416, -0.83364476, 0.25770026, -0.22943019],
                    [-0.20663183, 0.44842862, 0.43388028, -0.75363318],
                ],
                [
                    [0.31686704, -0.08984077, 0.84983632, 0.41146342],
                    [-0.68517756, -0.6155599, 0.30660825, -0.24001895],
                    [0.65344559, -0.55451806, -0.05416007, -0.51243075],
                    [0.05599871, -0.55274258, -0.42524851, 0.71449527],
                ],
                [
                    [0.31807464, -0.15700732, 0.85380344, 0.38104712],
                    [-0.74699539, -0.6476998, 0.147734, 0.02564219],
                    [-0.14995243, 0.13394936, -0.33097405, 0.92196964],
                    [-0.56421687, 0.73341069, 0.37369844, -0.06416816],
                ],
                [
                    [0.390439, -0.01875309, 0.86086611, 0.32575336],
                    [0.39536589, 0.90185327, -0.10792537, -0.13674282],
                    [-0.82880448, 0.4265968, 0.32413237, 0.16135834],
                    [0.06577493, 0.06575838, -0.37709396, 0.92149316],
                ],
                [
                    [0.38329812, -0.11468172, 0.86234845, 0.31029953],
                    [-0.65721304, -0.72204588, 0.13549772, 0.16840764],
                    [0.63834123, -0.68127882, -0.30917249, -0.18108569],
                    [0.11693607, 0.03683785, -0.37736969, 0.91791124],
                ],
                [
                    [0.46959679, 0.05831172, 0.80018924, 0.36847764],
                    [0.81593088, 0.12156139, -0.25553075, -0.50416631],
                    [0.04949337, 0.85203869, -0.29039923, 0.4327225],
                    [-0.33359566, 0.50581878, 0.4583334, -0.65022442],
                ],
            ]
        )

        # repeat 10 times with sampled data
        for i in range(SAMPLE_NUM_S):
            # prepare a target data
            indexes = rng.integers(0, _n_iris, size=SAMPLE_NUM_S)
            X = _iris.data[indexes]
            cov_mat = np.cov(X, rowvar=0, bias=1)

            # check the given module computes expected values
            eigenvalues_actual, eigenvectors_actual = compute_sorted_eigen(cov_mat)
            eigenvalues_expected = Eigenvalues_expected[i]
            eigenvectors_expected = Eigenvectors_expected[i]

            try:
                self.assertTrue(np.allclose(eigenvalues_actual, eigenvalues_expected))
                for i, v_expected in enumerate(eigenvectors_expected):
                    v_actual = eigenvectors_actual[i]
                    if not np.allclose(v_actual, v_expected):
                        self.assertTrue(np.allclose(-v_actual, v_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("covariance matrix:", arr2str(cov_mat))
                print("expected eigenvalues:", arr2str(eigenvalues_expected))
                print("actual eigenvalues:", arr2str(eigenvalues_actual))
                print("expected eigenvectors:", arr2str(eigenvectors_expected))
                print("actual eigenvectors:", arr2str(eigenvectors_actual))
                raise

    def test_4_MyFisher(self):
        C_DIGITS = 2
        TEST_RATIO = 0.05
        N_COMPONENTS = 16

        # prepare a target data
        cond_0 = _digits.target == 0
        cond_1 = _digits.target == 1
        data = _digits.data[cond_0 | cond_1]
        target = _digits.target[cond_0 | cond_1]
        X_train, X_test, y_train, _ = train_test_split(
            data, target, test_size=TEST_RATIO, random_state=RAND_SEED
        )

        # perform dimensionality reduction
        pca = PCA(n_components=N_COMPONENTS)
        pca.fit(X_train, y_train)
        X_train = pca.transform(X_train)
        X_test = pca.transform(X_test)

        # prepare expected results
        X_transformed_expected = np.array(
            [
                17.10398152,
                -21.16688411,
                -18.28434649,
                -3.91957131,
                18.15561078,
                -12.07046206,
                -8.48009826,
                18.49242294,
                -14.96392181,
                19.05348483,
                -16.17806568,
                11.45952864,
                18.66308975,
                17.16940529,
                -17.54786803,
                16.07027367,
                19.74824407,
                16.31320532,
            ]
        )

        # check the given model computes expected values
        model = MyFisher()
        model.fit(X_train, y_train)
        X_transformed_actual = model.transform(X_test)

        try:
            self.assertTrue(np.allclose(X_transformed_actual, X_transformed_expected))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Digits Dataset")
            print("  n_class:", C_DIGITS)
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("PCA's n_component:", N_COMPONENTS)
            print("expected:", arr2str(X_transformed_expected))
            print("actual:", arr2str(X_transformed_actual))
            raise

    def test_5_MyKL(self):
        C_DIGITS = 10
        TEST_RATIO = 0.005
        N_COMPONENTS = 10

        # prepare a target data
        X_train, X_test, _, _ = train_test_split(
            _digits.data, _digits.target, test_size=TEST_RATIO, random_state=RAND_SEED
        )

        # prepare expected results
        X_transformed_expected = np.array(
            [
                [
                    15.96598512,
                    5.26185844,
                    -15.79300261,
                    -4.27772703,
                    9.20056002,
                    14.03932305,
                    10.64841333,
                    -9.47798958,
                    -8.31688004,
                    41.91616154,
                ],
                [
                    -7.04034738,
                    -21.93931227,
                    4.60251748,
                    2.75131484,
                    10.31245685,
                    27.64570657,
                    -13.5896379,
                    -11.23231601,
                    -9.24307664,
                    39.1685608,
                ],
                [
                    -9.11175169,
                    22.83475198,
                    -9.31122151,
                    0.59437105,
                    6.2783056,
                    10.70963103,
                    -17.23145281,
                    11.35303286,
                    0.33261323,
                    33.63468094,
                ],
                [
                    -1.73535057,
                    12.97014857,
                    8.71296044,
                    -23.04698473,
                    -8.43335299,
                    24.70554606,
                    -8.71463557,
                    -24.62172531,
                    -7.65246683,
                    39.99745243,
                ],
                [
                    -21.31352611,
                    5.77636902,
                    -0.55116059,
                    6.89453958,
                    3.17603846,
                    25.49653846,
                    -13.34377645,
                    -25.12800393,
                    -15.76562259,
                    31.0565431,
                ],
                [
                    8.31128206,
                    7.02384616,
                    25.26989366,
                    -9.24959845,
                    1.72280579,
                    24.80988592,
                    -6.39074745,
                    -11.28418709,
                    -0.06194446,
                    22.8007771,
                ],
                [
                    -8.53516888,
                    9.85156086,
                    -3.43194803,
                    20.181552,
                    -6.34430746,
                    16.08948994,
                    -7.13303773,
                    -9.77051813,
                    -18.49622084,
                    36.02652427,
                ],
                [
                    -13.85103514,
                    -10.90802734,
                    -12.02525408,
                    -7.79495042,
                    0.08992726,
                    0.08707605,
                    -16.93927012,
                    -12.57381974,
                    -6.95379157,
                    35.13388292,
                ],
                [
                    16.48088749,
                    -10.95305939,
                    -2.93015685,
                    -12.04446809,
                    1.85977119,
                    9.5560599,
                    -11.10599676,
                    -3.24651962,
                    6.85059325,
                    38.6262304,
                ],
            ]
        )

        # check the given model computes expected values
        model = MyKL(n_components=N_COMPONENTS)
        model.fit(X_train)
        X_transformed_actual = model.transform(X_test)

        try:
            self.assertTrue(np.allclose(X_transformed_actual, X_transformed_expected))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Digits Dataset")
            print("  n_class:", C_DIGITS)
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("n_component:", N_COMPONENTS)
            print("expected:", arr2str(X_transformed_expected))
            print("actual:", arr2str(X_transformed_actual))
            raise


if __name__ == "__main__":
    unittest.main(argv=["dummy argument"], verbosity=2, exit=False)
