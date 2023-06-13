This program is an example of using TCP/IP to simulate a restaurant
This program also uses pthreads

BurgerJoint.cpp is the restaurant in which they have a limited amount of burgers to make total and a number of chefs (threads) that make the burgers
Customers can only get a burger if the restaurant has a burger already made
When Customers connect to the BurgerJoint they are handled with a thread allowing the burger joint to service multiple customers at a time

Customer.cpp is the customer wanting to eat at the restaurant, they will request burgers and will get a burger as long as the burger joint has a burger available, if a burger is not avaiable the customer will have to wait

When the BurgerJoint.cpp runs out a burgers to make and has no more burgers to sell they will inform all customers that they are out and the customers will disconnect and then the BurgerJoin will close.


To run this program first run BurgerJoint.cpp and then run as many instances of Customer.cpp as you like they are connected to each other by TCP/IP and communicate by sending messages to each other




