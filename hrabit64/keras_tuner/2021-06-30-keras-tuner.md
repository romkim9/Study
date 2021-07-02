---
title: 케라스 튜너를 이용해 하이퍼 파라미터를 최적화 해보자!
categories: Tensorflow
tags: [Tensorflow,keras-tuner]
toc: true
toc_label: 목차
toc_sticky: true
classes: wide
---



# 0. 개요

 **이 글에서 작성한 코드는 Git(https://github.com/hrabit64/keras_tuner_exercise)에 올려져 있습니다!**

머신러닝을 공부하다 보면 하이퍼 파라미터란 용어를 듣게 될 것이다. 구글의 Tensorflow 문서에서는 이 하이퍼파리미터를 ***"훈련 프로세스 및 ML 모델의 토폴로지를 제어하는 변수입니다."*** 라고 정의하고 있다. 쉽게 말해 파라미터는 ML이 결정하는 요소이고, 하이퍼 파라미터는 사용자가 정해줘야 하는 요소라고 생각하면 된다.

  이 하이퍼 파라미터에 따라 모델의 성능이 달라지기 때문에 이 하이퍼 파라미터를 최적화하는 것은 상당히 중요하다. 이를 최적화 하는 기법에는 **Random search**,**Grid search**,**Hyper Band**,**Hyperparameter Optimization** 등의 기법들이 존재하지만 거의 공통적으로 동일 한 것은 하이퍼 파라미터의 값의 범위를 설정하고 서서히 그 값들을 줄여나가는 방식이라고 생각하면 된다.

 하지만 이러한 기법들을 직접 일일이 구현하기에는 귀찮고, 혹은 직접 텐서보드를 보면서 일일이 작업하기에는 너무 비효율적이다. 그걸 아는지 케라스에서 이 케라스 튜너라는 좋은 라이브러리를 만들어 주었다.

 케라스 튜너는 위에서 언급한 다양한 최적화 기법을 사용할 수 있고, 단순히 하이퍼 파라미터를 최적화하는 것을 넘어 모델의 레이어 갯수를 결정하거나, 더욱 적합한 기법을 찾는 등 상당히 무긍무진한 활용법을 가지고 있다.

 다만 이 글을 작성하고 있는 날 기준으로 잔 버그가 많다고 느끼니 언젠가 나올 1.0.4 버전을 기대해보는 것도 좋을 것 같다.



 그럼 지금부터 간단하게 작성된 CNN 모델을 최적화하는 과정을 따라가 보자.



# 1. 하이퍼 모델

케라스 튜너를 통해 최적화하고 싶은 모델이 있다면, 그 모델을 하이퍼 모델로 설정해주어야 한다. 이 하이퍼 모델은 기존 모델처럼 신경망 등이 정의 되어야 하고, 하이퍼 파라미터들의 검색 범위 또한 지정되어야 한다.

아래는 기본적인 CNN 모델이다.

```python
	model.add(Conv2D(12, (5,5), padding="same", input_shape=Input_Shape, activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(Dropout(0.25))
    
    model.add(Conv2D(16, (5,5), padding="same", activation='relu'))
    model.add(MaxPooling2D(pool_size=(2,2)))
    model.add(Dropout(0.25))
    
    model.add(Flatten())
    model.add(Dense(128, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(Count_Of_Categories, activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
```

위 모델에서 내가 최적화하고자 하는 부분은 Conv2D의 필터 수와 Dense 층의 유닛수이다. 이 부분들에 대한 탐색범위를 지정해주어야 하는데, 둘다 정수 범위에서 검색할 것이므로, Int 메소드를 사용할 것이다. https://keras.io/api/keras_tuner/ 에 다양한 메소드들이 있으니, 본인이 탐색하고자 하는 검색범위와 값에 맞추어 사용하자.

다음과 같이 하이퍼 모델을 정의해주었다.

```python
def build(hp):

    model = Sequential()
    model.add(Conv2D(hp.Int('L1_Conv2D_Filters',min_value=8,max_value=64,step=4)
                     ,kernel_size=(5, 5), padding="same", input_shape=Input_Shape, activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))

    model.add(Conv2D(hp.Int('L2_Conv2D_Filters',min_value=8,max_value=64,step=4)
                     ,kernel_size= (5, 5), padding="same", activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))

    model.add(Flatten())
    model.add(Dense(hp.Int("L3_Dense_num",min_value=4,max_value=216,step=4)
                    , activation='relu'))
    model.add(Dropout(0.25))
    model.add(Dense(Count_Of_Categories, activation='softmax'))
    model.compile(loss='categorical_crossentropy', optimizer='adam', metrics=['accuracy'])


    return model
```

우선 하이퍼 모델은 필수적으로 하이퍼 파라미터를 인자로 받아야 한다. 위 코드에서는 hp가 하이퍼 파라미터이다.

```python
    model.add(Conv2D(hp.Int('L1_Conv2D_Filters',min_value=8,max_value=64,step=4)
                     ,kernel_size=(5, 5), padding="same", input_shape=Input_Shape, activation='relu'))
```

위 코드에서는 정수값을 찾으며 8~64 범위로 간격은 4로 지정되어 있다. 우선 첫번째 인자로 파라미터의 이름을 지정해주어야 한다. 케라스 API 문서에서는 다른 이름과 겹치지 않게 고유한 이름으로 작성하라고 명시되어있다.

그 다음은 검색할 최솟값과 최댓값 그리고 값 사이의 간격인 step을 설정해주면 된다. 다른 인자들도 더 설정가능하니 케라스의 https://keras.io/api/keras_tuner/hyperparameters/#int-method 이 부분을 참고해보자



# 2. 튜너 설정하기

위에서 만든 하이퍼 모델을 최적화 해주는 친구를 케라스 튜너에서는 튜너라고 부른다. 오늘 이 블로그에서는 RandomSearch를 활용해보겠다.

```python
tuner = kt.RandomSearch(build,
                        objective = 'val_loss',
                        max_trials =50,
                        directory="log",
                        project_name="test")
```

먼저 첫번째 인자에는 우리가 위에서 만든 하이퍼 모델의 함수명을 입력해주면 된다. 다음 인자인 objective는 최고의 모델을 찾는데 사용할 기준이다. 이 코드에는 손실률이 가장 낮은 모델이 최고의 모델이 될 것이다. directory에는 로그가 기록될 폴더명을 입력해주면 된다.



학습하기 전, 우리가 설정한 하이퍼 파라미터에 대한 정보를 보여주게 해보자

```python
tuner.search_space_summary()
```

을 사용해주면 우리가 설정한 하이퍼 파라미터에 대한 정보를 볼 수 있다.



마지막으로

```python
tuner.search(X_train, Y_train, epochs = 10, validation_data = (X_test, Y_test))
```

을 입력해주면 학습이 되는 모습을 볼 수 있다. 이때 사용되는 인자들은 기존 케라스의 fit과 동일하다.



학습이 종료되었다면

```python
tuner.results_summary()
```

아래와 같이 입력해준다면 결과를 확인할 수 도 있다.



마지막으로 최상의 모델을 선택하는 방법에는 2가지 방식이 있는데, 학습이 된 최고의 모델을 가져와 다시 학습시키는 방법, 혹은 최상의 파라미터를 가져오는 방법이 있는데, 케라스의 api 문서에서는 파라미터를 가져오는 방식을 추천하고 있다.



```python
get_best_model = tuner.get_best_hyperparameters()[0]
model = tuner.hypermodel.build(get_best_model)
```

다음과 같이 최적화시킨 하이퍼파라미터를 가져와 model에 적용시킬 수 있다.

이렇게 최적화된 모델을 이용해 기존 학습과 동일하게 학습시키고 활용하면 된다.