# Programming Assignment 3

Naive Bayes Text Classifier 

## Introduction

It is a C program for predicting whether the sentiment of a given sentence is negative or not

• trainer: build a Naive Bayes model from the Twitter Airline dataset

• predictor: read a sentence and returns either ‘Negative’ or ‘Non-negative’

### To run the program

First go to "example" repository.

```
cd example
```

compile all the programs.

```
make
```

### Trainer

It reads the given data files and generate model.csv which is a Naive Bayes model for the sentiment classification learnt from the given dataset

```
./count
```

### Predictor

It reads  model.csv and receive a sentence (one line of text) from the standard input to print out the prediction result as either ‘Negative’ or ‘Nonnegative’

```
./predictor
```
For example, 

```
./predictor
Your sentence : The chairs were dirty and people were rude <- user input (stdin)
 
The sentence is Negative
```


## Team Members 

Team32 : 
* 김영빈(21600108)
* 손한나(21800389)
* 윤수빈(21800491) 
* 이민재(21800511)
* 정수산나(21800662)
