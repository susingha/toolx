Memorable but incommunicable passwords

This project is inspired by two recent papers:

    Neuroscience Meets Cryptography: Designing Crypto Primitives Secure Against Rubber Hose Attacks, Hristo Bojinov, Daniel Sanchez, Paul Reber, Dan Boneh, and Patrick Lincoln.
    PhorceField: A Phish-Proof Password Ceremony, Michael Hart, Claude Castille, Manoj Harpalani, Jonathan Toohill, and Rob Johnson. 

The first paper designs an authentication scheme based on skill, i.e. users are able to login if they can demonstrate sufficient skill at playing a game. Since it is very difficult for them to transfer skill to another person, they are immune to threats asking them to reveal their authentication information.

The second paper describes a password scheme based on recognizing images. When presented with a small set of images containing the images in their password, users can log in easily. However, without that prompt (say, when visiting a phishing site), users have a difficult time communicating their password to the server.

This project envisions extending the ideas in the PhorceField paper to see if it is possible to create graphical passwords that users can remember and use to login, given a prompt, but cannot easily communicate to another person. The PhorceField paper demonstrated that users cannot easily communicate their password when using a search-based interface, but it is not clear if they cannot communicate their password using any interface.

For this project, we collect two or more sets of images, i.e. images of concrete nouns (car, horse stapler, etc), images of human faces, images of landscapes, randomly-generated images, or some other classes of images. Each set contains at least 100 images.

We then perform a user-study to evaluate each set of images on two criteria: recognizability and communicability.

To measure recognizability, we assign passwords consisting of a sequence of images from a single class to participants. We test the participants over a week or two to measure how well they are able to remember their password. We expect that some passwords are easier to remember than others. For example, passwords consisting of faces are more memorable than passwords consisting of randomly-generate art or landscapes.

To measure communicability, we design a two-player system where one subject (the sender) attempts to communicate one or more images in his password to another subject (the recipient). We provide the subjects with several communication channels, such as:

    A search interface to the database of images
    The ability to show the other subject an image from a collection of search results
    A drawing tool
    Typed instance messages
    Voice communication
    Video communication 

We measure how long it takes the sender to lead the recipient to find the sender's image. We measure success rates, in case some of the subjects fail to find the image.

