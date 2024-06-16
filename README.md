# Word2Vec C++ Implementation

This project contains a C++ implementation of the Word2Vec model. It includes functionality for training word embeddings and using those embeddings to perform inference tasks such as finding the most similar words based on cosine similarity.

## Project Structure

- `src/`: Contains all the C++ source files.
- `include/`: Contains all the header files.
- `bin/`: Contains the compiled executables.
- `obj/`: Contains intermediate object files.
- `Makefile`: Defines the build instructions.
- `.gitignore`: Specifies intentionally untracked files to ignore.

## Prerequisites

- C++ Compiler supporting C++11 (e.g., GCC, Clang)
- Make (for using the Makefile)

## Compilation

To compile the project, use the provided Makefile. Open your terminal, navigate to the project directory, and run:

```bash
make
```

This command will compile all source files and generate two executable files:

`train_word2vec` for training the Word2Vec model.
`inference_word2vec` for running inference tasks like finding the most similar words.

## Running the Executables

**Training**
To train the Word2Vec model, you need to provide a text file as the training corpus. You can run the training executable with command-line arguments to specify the corpus path and optionally the hyperparameters like window size, vector size, learning rate, and number of epochs.

## Example:

```bash
./bin/train_word2vec --corpus "path/to/corpus.txt" --window 5 --size 100 --rate 0.025 --epochs 10
```

**Inference**
To use the trained model for inference (e.g., finding the most similar words):

```bash
./bin/inference_word2vec --vector "path/to/vectors.json" --word "king"
```

This command will output the word most similar to "king" based on the trained vectors.

Hyperparameters
The command-line arguments for both training and inference are:

- `--corpus`: Path to the training corpus.
- `--window`: The size of the context window (default is 2).
- `--size`: Dimension of the word vectors (default is 100).
- `--rate`: Learning rate (default is 0.025).
- `--epochs`: Number of training epochs (default is 10).
- `--vector`: Path to the JSON file containing word vectors (for inference).