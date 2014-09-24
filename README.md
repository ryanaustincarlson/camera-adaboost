webcam-adaboost
===============

Train a model on a set of camera images (or any images, really) to discern what is "interesting" about the images, then use the model to classify incoming images as "interesting" or "uninteresting". This was originally intended for combing through a whole bunch of webcam footage to check if someone was out and about when you're not home, though it can be used for a much wider set of things.

This project is NOT in active development -- that ended in like 2009ish. I dug this up to post here by request. It contains a full implementation of the adaboost algorithm (which you can also find here: https://github.com/astromme/AdaBoost), and a GUI written in Qt to help train the model. 

The adaboost algorithm requires Boost (http://www.boost.org/), and the GUI obviously requires Qt (http://qt-project.org/).
