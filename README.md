# VectorDB-words
- - -

This is a project to implement a vector-database for English words

I have used only parquet and nlohmann-json libraries to read parquet and json files
other than this nothing else is used to ensure you have the same present in your /usr/include or wherever other cpp libs are located for you.

to run this just make it and cd to build dir and run main

also remember to set the dataset directories according to your download paths

this is the initial dir structure that is expected by default:


``` typescript
.
├── build
├── dataset
│   ├── cleaned
│   └── download
│       └── medium_articles
│           ├── data
│           │   ├── train-00000-of-00003-30c95f783f2586d1.parquet
│           │   ├── train-00001-of-00003-55f3177d76922e15.parquet
│           │   └── train-00002-of-00003-d38a4e3be95b53b3.parquet
│           └── README.md
│
├── Makefile
├── Makefile.bak
├── models
├── README.md
└── src
    ├── include
    │   ├── data_handle.cpp
    │   ├── data_handle.hpp
    │   ├── glove.cpp
    │   ├── glove.hpp
    │   ├── parquet_handle.cpp
    │   └── parquet_handle.hpp
    └── main.cpp
```

after building and running this will be your dir structure

``` typescript
.
├── build
│   ├── main
│   └── src
│       ├── include
│       │   ├── data_handle.o
│       │   ├── glove.o
│       │   └── parquet_handle.o
│       └── main.o
├── dataset
│   ├── cleaned
│   │   └── medium_articles.json
│   └── download
│       └── medium_articles
│           ├── data
│           │   ├── train-00000-of-00003-30c95f783f2586d1.parquet
│           │   ├── train-00001-of-00003-55f3177d76922e15.parquet
│           │   └── train-00002-of-00003-d38a4e3be95b53b3.parquet
│           └── README.md
│
├── Makefile
├── Makefile.bak
├── models
│   └── glove_model.json
├── README.md
└── src
    ├── include
    │   ├── data_handle.cpp
    │   ├── data_handle.hpp
    │   ├── glove.cpp
    │   ├── glove.hpp
    │   ├── parquet_handle.cpp
    │   └── parquet_handle.hpp
    └── main.cpp
```

## steps for setup
1. clone the repo
	``` bash
   git clone https://github.com/hermanumrao/VectorDB-words.git 
   cd VectorDB-words
	```

2. create required dirs
	```bash
   mkdir -p build dataset/cleaned dataset/download/ models
	```
4. download the required dataset using git-lfs
	``` bash
	cd dataset/download/
	# Make sure you have git-lfs installed (https://git-lfs.com)
	git lfs install
	git clone https://huggingface.co/datasets/fabiochiu/medium-articles
	cd ../..
 	```
5. compile the project
	``` bash
   make
	```
6. run the file
	```
   ./build/main
	```
