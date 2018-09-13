Jumble Game
===========

![Gumble main Screenshot](https://raw.githubusercontent.com/Cabrra/cabrra.github.io/master/Images/Gumble-1.jpg?token=AI_RbUvR6k793fvlqr97SqRn-VzmMEVNks5bo5zkwA%3D%3D)

This fun project is a 6 letter word jumble game. The player is presented with 6 letters and the objective is to think of as many words as possible that are made up of those 6 letters in a given amount of time. 

## Controls and files

+ engldict: 			Contains a basic list of words that will be legal answers for the game.
+ Keyboard keys: 		Pressing the keyboard letters will use the pressed letter and remove it from the available list.

## Gameplay

The game starts each round finding a random 6 letter word in the hash table, storing and scrambling the letters that form the word, and displaying them to the screen. The player will have 60 seconds to make as many words as possible using those 6 letters. Each letter position can be used only once. If the player’s guesses are real words (in the dictionary) and made up of the letters in the scramble, the word will be stored and points will be awarded for a correct guess (The score for a word is the factorial of its length-1).

Game-Flow:
+ The dictionary is loaded at launch and the legal words for the game are stored in a hash table (custom hash function)  
    The hash table is using at least 500 buckets  
    As a requirement, no more than 5% of the buckets are empty (check le output log)  
    Items are distributed as evenly among the buckets  
+ Not storing words shorter than 3 letters or longer than 6 letters (because those don't give points to the player)
+ Not storing words with an apostrophe (‘) in it
+ Remaining time in the round is displayed on screen (real time countdown)
+ Current score displayed to the screen
+ Player’s correct guesses are stored in a Binary Search Tree and displayed to the screen
+ The solution is displayed on the screen, if the player does not guess the word by the end of the round
+ Top 5 high scores stored in a binary file  
    The high score table is displayed at the end of a game  
    The scores are stored as: name + score (displayed in descending order)  

## Prerequisites

This project is based on some custom data structures implemented on one of my [repos](https://github.com/Cabrra/Data-Structures).

## Built With

* [Visual Studio](https://visualstudio.microsoft.com/)	- C++ development
* Run with Windows CMD

## Contributing

Please read [CONTRIBUTING.md](https://github.com/Cabrra/Contributing-template/blob/master/Contributing-template.md) for details on the code of conduct, and the process for submitting pull requests to me.

## Authors

* **Jagoba "Jake" Marcos** - [Cabrra](https://github.com/Cabrra)

## License

This project is licensed under the MIT license - see the [LICENSE](LICENSE) file for details

## Acknowledgments

* Full Sail University - Game Development Department
* Rodney Stoeffler
