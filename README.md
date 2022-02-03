Toward a Music Social Network The basic goal of the assignment is to design and implement a simple music recommendation system that uses social connections of users to suggest songs to listen to.

This assignment will use Songs as the basic objects on which it operates. But once again, we are not building on the functionality of previous assignments; instead, we are doing something new and different from previous assignments. Your program does not need to support anything other than the operations described below.

Working in Teams

You must work in a group of 3 for this assignment. (A minimum number of groups may be altered to contain 2 or 4 members; I (Mike) will initiate and decide which teams will contain 2 or 4 members, instead of 3.)

Overview

As with previous assignments, your program will model users who establish collections (libraries) of songs. However, you only need to maintain a single library of songs for one of the users (called the Primary User). The notion of a playlist is not necessary for this assignment. Each user has a collection of friends, and the goal of the program is to make song recommendations for the Primary User, based on what that user's friends are listening to. The system does not have to recommend songs for users other than the Primary User. In this system, when a recommendation is made, the recommended song is added to the Primary User's library.

Functionality

The following functionality must be supported, but we are not prescribing the exact interface to your program this time. You should design that yourself. I expect that most groups will create a menu style interface that resembles the style of CA2 and CA3, but that's up to you. Because we do not have a common interface that every program must adhere to, you will demo your program to a TA or Mike for grading, and you should build an extensive set of test cases that proves that your code works well and works efficiently. We will also give you descriptions of tests (more on that coming later) that we would like to see; your program must support that description by implementing operations for each of the items below.

Your program should be able to:

1. Add a new user to the system

2. Make a friendship connection between two users, if that friendship does not yet exist. You should support friendship connections between the Primary User and other users, and also between pairs of other users.

3. Break a friendship connection between two specified users, if one exists.

4. Add a new song to the system. It is highly recommended that you reuse functionality to read songs from files, as you did in CA3. This will allow you to add many songs with minimal typing, and is more convenient than piping data and reading from standard input (ala CA2). If none of your group members has code that does this well (unlikely!), we will give you code to read songs from files. Note that this operation adds a song to "the system", but not to the Primary User's library. The only way that songs can be added to the Primary User's library is through the recommendation system (see below).

5. Have a user listen to a song N times. If the song is in the Primary User's library, this operation has no real effect. If the song is not in the Primary User's library, then the Primary User cares about this information if and only if the user who listens is within the "sphere of influence" of the Primary User. Basically, the goal of the Music Social Network is to track which songs the Primary User's friends (and potentially friends of friends... and friends of friends of friends, etc.) are listening to, in order to bring those songs to the attention of the Primary User, by adding them to the Primary User's library, as described further below. We will call this sphere of influence the Extended Friendship Network (EFN) of the Primary User.

You may assume that if a user listens to a song, then that song is in that user's library. This means that you only have to have one library of songs, namely for the Primary User.

6. Set the EFN radius to K, where K is a small positive number. This operation influences which song listens, by friends, influence the Primary User.
Importantly (and this makes things easier for you), the Primary User only cares about song listens that happen while a user is within the Primary User's EFN. (a) Suppose the EFN radius is 1, and George is a friend of a friend, but not a friend, of the Primary User. If George listens to a song 10 times, then becomes a friend of the Primary User, and then listens to the song another 5 times, the system should only reflect the 5 recent listens, not all 15, when deciding recommendations. (b) Likewise, if Adriana listens to a song 10 times, then a friendship connection is established that puts her into the EFN of the Primary User, and then she listens to the song another 5 times... only the 5 most recent listens matter, not the initial 10. (c) Similarly, if Leo is within the Primary User's EFN, and then Leo stabs someone in the back and a friendship is broken that takes Leo out of the EFN, your program should not try to "undo" any of Leo's previous listens.

Also important: Please implement this program for EFN = 1 only, first. That is significantly easier than supporting a general ENF radius of K. You can add that functionality later.

7. Recommend and add to the Primary User's library, the top K songs that users from the Primary User's EFN have been listening to. More specifically, this operation should add the K songs that are not in the Primary User's library, and that have the most listens by users while they were in the Primary User's EFN. K should be selected by the user of your program. (In other words, whoever runs your program should be able to select the top 3 songs with one operation and then the top 5 with the next, substituting any positive integer for K each time). This should print out the song info for all the recommended songs and automatically add them to the Primary User's library.

8. Remove a song from the Primary User's library. This song may later be recommended to the user, but only if it gets enough new listens within the EFN. So when a song moves into the Primary User's library, then gets removed, it's listen count should be reset to 0. Once a song is added to the system (as opposed to the Primary User's library), it cannot be removed.

9. Show a list of friends, for the Primary User or for other users, as indicated by the user of your program.

10. Show songs in the Primary User's library.

11. Show songs that are in the system but not in the Primary User's library, along with the number of relevant listens by users in the EFN. You may want to allow the user of your program to limit the number of songs that are shown (like the show() function from CA4).

Implementation Requirements

Efficiency matters!! You should strive to support these operations as efficiently as possible.

In particular, nothing (other than showing songs) should take tight upper bound O(S) time, where S is the number of songs. Everything should be asymptotically more efficient than that.

Nothing should take tight upper bound O(U) time where U is the number of users (assuming that the number of friends of any user is much smaller than U, and that the EFN is much smaller than U); everything should be asymptotically more efficient than that.

Think O(log N)

PLEASE DO NOT BUILD BALANCED BINARY SEARCH TREES. Use REGULAR binary search trees and you may ASSUME thay are balanced. In other words, if you build a BST, then insert, find, and remove all "count" for the purposes of this assignment as O(log N) operations, even though technically they are not worst case O(log N).
