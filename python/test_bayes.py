import numpy as np
import pandas as pd
import unittest

from sklearn.feature_extraction.text import CountVectorizer
from sklearn.model_selection import train_test_split

# ===== ColabないしJupyter上で実行する際は消す ===== #

from bayes import compute_log_prior_p, compute_log_post_P, compute_likelihoods, MyBayes

# ===== ここまで ===== #

# global test settings
RAND_SEED = 1289
SAMPLE_NUM_S = 10
SAMPLE_NUM_M = 100
SAMPLE_NUM_L = 1000
CLASS_NUM = 2
DIM_NUM = 5
DUMMY_LOG_WEIGHT = -3


def arr2str(arr):
    return "\n" + np.array2string(arr, separator=", ")


class BayesTestCase(unittest.TestCase):
    def __init__(self, *args, **kwargs):
        super(BayesTestCase, self).__init__(*args, **kwargs)

    def test_1_compute_log_prior_p(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare expected results
        log_P_expected = np.array(
            [
                [-0.51082562, -0.91629073],
                [-0.91629073, -0.51082562],
                [-0.51082562, -0.91629073],
                [-0.51082562, -0.91629073],
                [-0.51082562, -0.91629073],
                [-0.91629073, -0.51082562],
                [-0.69314718, -0.69314718],
                [-1.20397280, -0.35667494],
                [-0.51082562, -0.91629073],
                [-2.30258509, -0.10536052],
            ]
        )

        # repeat 10 times with sampled data
        for i in range(SAMPLE_NUM_S):
            # prepare a target data
            y = rng.integers(0, CLASS_NUM, size=SAMPLE_NUM_S)

            # check the given module computes expected values
            log_p_actual = compute_log_prior_p(y)
            log_p_expected = log_P_expected[i]
            try:
                self.assertTrue(np.allclose(log_p_actual, log_p_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("y:", arr2str(y))
                print("expected:", arr2str(log_p_expected))
                print("actual:", arr2str(log_p_actual))
                raise

    def test_2_compute_log_post_P(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare expected results
        log_post_P_expected = np.array(
            [
                [
                    [-1.39665715, -1.80212226, -2.08980433, -1.39665715, -1.53018854],
                    [-1.56861592, -1.74296931, -1.29098418, -1.38629436, -2.38959647],
                ],
                [
                    [-1.51198258, -1.37845118, -1.62531126, -2.01097374, -1.62531126],
                    [-1.97716269, -1.64865863, -1.57169758, -1.22994829, -1.776492],
                ],
                [
                    [-1.5699591, -2.11357455, -1.63665048, -1.31506685, -1.5699591],
                    [-1.41098697, -1.76766192, -1.51634749, -1.51634749, -1.9218126],
                ],
                [
                    [-1.55644538, -1.69459572, -1.85493837, -1.81237876, -1.25276297],
                    [-1.58240924, -1.51787072, -1.58240924, -1.51787072, -1.89256417],
                ],
                [
                    [-1.41908418, -1.56218503, -1.48807706, -1.82454929, -1.82454929],
                    [-1.31686585, -1.89494371, -1.53393036, -1.8115621, -1.59645072],
                ],
                [
                    [-1.97606319, -1.31928365, -1.61938824, -1.61938824, -1.61938824],
                    [-1.39285176, -1.85238409, -1.59645072, -1.69823341, -1.56470202],
                ],
                [
                    [-1.99243016, -1.21924028, -1.43281438, -1.99243016, -1.64412347],
                    [-1.76234558, -1.4638526, -1.95340082, -1.67896398, -1.31432086],
                ],
                [
                    [-1.6405285, -2.18707221, -1.1837701, -1.6405285, -1.6405285],
                    [-1.9625772, -1.57691472, -1.1322289, -1.70474809, -1.90541879],
                ],
                [
                    [-1.40282366, -1.58514522, -1.75949861, -1.33828514, -2.16496372],
                    [-1.79175947, -1.60070423, -1.645156, -2.02814825, -1.18085039],
                ],
                [
                    [-1.5040774, -1.61530303, -2.00616934, -1.61530303, -1.40399394],
                    [-1.23676263, -1.56218503, -1.92990981, -1.82454929, -1.64222774],
                ],
            ]
        )

        # repeat 10 times with sampled data
        for i in range(SAMPLE_NUM_S):
            # prepare a target data
            X = rng.integers(0, SAMPLE_NUM_S, size=(SAMPLE_NUM_S, DIM_NUM))
            y = rng.integers(0, CLASS_NUM, size=SAMPLE_NUM_S)

            # check the given module computes expected values
            log_P_actual = compute_log_post_P(X, y)
            log_P_expected = log_post_P_expected[i]
            try:
                self.assertTrue(np.allclose(log_P_actual, log_P_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("X:", arr2str(X))
                print("y:", arr2str(y))
                print("expected:", arr2str(log_P_expected))
                print("actual:", arr2str(log_P_actual))
                raise

    def test_3_compute_likelihoods(self):
        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare expected results
        Likelihoods_expected = np.array(
            [
                [
                    [-40.84103499, -42.03704614],
                    [-22.50065935, -38.78310327],
                    [-49.66529989, -63.06031132],
                    [-20.31832674, -34.84927289],
                    [-54.01693652, -58.4496301],
                    [-14.13881313, -25.24460696],
                    [-47.35526955, -51.85971277],
                    [-44.68582594, -35.53135859],
                    [-48.91971514, -52.74596375],
                    [-52.12758931, -50.6685873],
                ],
                [
                    [-48.55126346, -46.72873602],
                    [-26.90067153, -27.61688795],
                    [-44.94854098, -32.73108131],
                    [-45.54240681, -41.39538413],
                    [-78.32197136, -71.04469255],
                    [-43.20167112, -31.21480145],
                    [-41.56722268, -44.42131537],
                    [-25.81413491, -27.41575779],
                    [-45.28305629, -34.93854853],
                    [-59.28575662, -58.96140098],
                ],
                [
                    [-34.67063948, -36.73194234],
                    [-45.62208881, -33.08285094],
                    [-30.74793463, -33.65465702],
                    [-45.03793124, -49.1981221],
                    [-44.50233935, -42.63381469],
                    [-24.87287725, -30.36444117],
                    [-27.58532504, -28.44895035],
                    [-57.0531149, -61.56490096],
                    [-41.8042393, -44.89342274],
                    [-16.45369382, -18.22609075],
                ],
                [
                    [-12.29845076, -14.26706529],
                    [-33.82700339, -19.41601621],
                    [-36.3796226, -24.31396525],
                    [-10.13133333, -7.63522053],
                    [-50.4464485, -34.72995328],
                    [-60.27839655, -49.30958883],
                    [-52.21586014, -29.71176922],
                    [-56.56692055, -45.30418338],
                    [-64.10724506, -45.9468148],
                    [-34.52256708, -23.90199352],
                ],
                [
                    [-33.42403694, -45.91803339],
                    [-47.9060453, -44.95392974],
                    [-60.94691672, -62.63092847],
                    [-34.39454235, -35.98067432],
                    [-30.81526096, -24.07266996],
                    [-35.87058689, -29.91590133],
                    [-47.0453385, -53.11706855],
                    [-29.16035607, -41.37330047],
                    [-28.59868167, -21.48093885],
                    [-52.03490251, -52.7217366],
                ],
                [
                    [-34.38283438, -29.33927141],
                    [-24.57573799, -20.80615615],
                    [-24.88876402, -27.17729677],
                    [-28.23677874, -23.63356257],
                    [-34.2666891, -31.46456294],
                    [-35.14474113, -31.34523297],
                    [-51.66195823, -38.6045362],
                    [-40.67566365, -32.1196563],
                    [-26.01745805, -23.89245978],
                    [-49.42359575, -40.53614087],
                ],
                [
                    [-50.74433997, -52.44430753],
                    [-53.45267145, -51.39001545],
                    [-13.46368409, -17.10088532],
                    [-30.41688794, -29.82331592],
                    [-21.75578476, -29.46502801],
                    [-45.60558305, -34.91887135],
                    [-25.73238613, -19.9237479],
                    [-32.28477038, -24.40773481],
                    [-68.24959153, -55.64033628],
                    [-62.29562545, -30.34959027],
                ],
                [
                    [-18.93812838, -27.17180299],
                    [-26.15738648, -27.13356475],
                    [-27.06610548, -27.8572018],
                    [-53.83602564, -61.88687422],
                    [-21.4404986, -25.04958489],
                    [-11.20444623, -26.21867499],
                    [-42.63819411, -36.7331009],
                    [-39.48233676, -42.55716941],
                    [-40.14730083, -44.34134374],
                    [-35.99608516, -43.8678589],
                ],
                [
                    [-19.89553979, -15.92042843],
                    [-29.42203388, -41.20068],
                    [-15.72767184, -30.05320129],
                    [-29.5552562, -31.09882106],
                    [-20.91158652, -24.87414712],
                    [-26.53956843, -38.67594831],
                    [-31.56757381, -42.60338938],
                    [-16.51833829, -27.66806335],
                    [-30.56276753, -47.13870484],
                    [-22.11690844, -34.53606401],
                ],
                [
                    [-54.72971112, -33.69430072],
                    [-42.11788771, -18.86422875],
                    [-59.18035727, -44.59862011],
                    [-36.01945515, -24.1450595],
                    [-34.93809633, -15.95493477],
                    [-34.24010567, -31.79904371],
                    [-21.31923779, -12.36380313],
                    [-61.6112648, -33.39394744],
                    [-38.38606754, -23.4804481],
                    [-19.77977507, -14.4850609],
                ],
            ]
        )

        # repeat 10 times with sampled data
        for i in range(SAMPLE_NUM_S):
            # prepare a target data
            X = rng.integers(0, SAMPLE_NUM_S, size=(SAMPLE_NUM_S, DIM_NUM))
            log_prior_p = rng.random(CLASS_NUM) * DUMMY_LOG_WEIGHT
            log_post_P = rng.random((CLASS_NUM, DIM_NUM)) * DUMMY_LOG_WEIGHT

            # check the given module computes expected values
            likelihoods_actual = compute_likelihoods(X, log_prior_p, log_post_P)
            likelihoods_expected = Likelihoods_expected[i]
            try:
                self.assertTrue(np.allclose(likelihoods_actual, likelihoods_expected))
            except:
                print("\n\nA unittest was failed with the following values:")
                print("X:", arr2str(X))
                print("log_prior_p:", arr2str(log_prior_p))
                print("log_post_P:", arr2str(log_post_P))
                print("expected:", arr2str(likelihoods_expected))
                print("actual:", arr2str(likelihoods_actual))
                raise

    def test_4_MyBayes(self):
        TEST_RATIO = 0.2
        DUMMY_NUM = 50
        HALF_DUMMY_NUM = int(DUMMY_NUM / 2)
        SPAM_WEIGHT = np.array([0, 0, 0, 5, 5])

        # prepare a random generator with a fixed seed
        rng = np.random.default_rng(seed=RAND_SEED)

        # prepare a target data
        X_0 = rng.integers(0, SAMPLE_NUM_S, size=(HALF_DUMMY_NUM, DIM_NUM))
        X_1 = rng.integers(0, SAMPLE_NUM_S, size=(HALF_DUMMY_NUM, DIM_NUM)) + SPAM_WEIGHT
        X = np.vstack((X_0, X_1))
        y = np.array([0 if i < HALF_DUMMY_NUM else 1 for i in range(DUMMY_NUM)])
        X_train, X_test, y_train, _ = train_test_split(
            X, y, test_size=TEST_RATIO, random_state=RAND_SEED
        )

        # prepare expected results
        y_expected = np.array([1, 0, 0, 0, 0, 1, 1, 1, 0, 0])

        # check the given model computes expected values
        model = MyBayes()
        model.fit(X_train, y_train)
        y_actual = model.predict(X_test)

        try:
            self.assertTrue(np.allclose(y_actual, y_expected))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: Dummy Dataset")
            print("  X:", arr2str(X))
            print("  y:", arr2str(y))
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("expected:", arr2str(y_expected))
            print("actual:", arr2str(y_actual))
            raise

    def test_5_SMSSpamCollection(self):
        TEST_RATIO = 0.01

        # prepare a target data
        spam_df = pd.read_csv("SMSSpamCollection", sep="\t", header=None)
        messages = spam_df[1]
        cv = CountVectorizer(min_df=3)
        cv.fit(messages)
        X = cv.transform(messages).toarray()
        y = np.where(spam_df[0] == "spam", 1, 0)
        X_train, X_test, y_train, _ = train_test_split(
            X, y, test_size=TEST_RATIO, random_state=RAND_SEED
        )

        # prepare expected results
        y_expected = np.array(
            [
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                1,
                0,
                0,
                0,
                0,
                1,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                1,
                1,
            ]
        )

        # check the given model computes expected values
        model = MyBayes()
        model.fit(X_train, y_train)
        y_actual = model.predict(X_test)

        try:
            self.assertTrue(np.allclose(y_expected, y_actual))
        except:
            print("\n\nA unittest was failed with the following values:")
            print("dataset: SMS Spam Collection Data Set")
            print("  test_size:", TEST_RATIO)
            print("  random_state:", RAND_SEED)
            print("expected:", arr2str(y_expected))
            print("actual:", arr2str(y_actual))
            raise


if __name__ == "__main__":
    unittest.main(argv=["dummy argument"], verbosity=2, exit=False)
