webcam-adaboost
===============

Train a model on a set of camera images (or any images, really) to discern what is "interesting" about the images, then use the model to classify incoming images as "interesting" or "uninteresting". This was originally intended for combing through a whole bunch of webcam footage to check if someone was out and about when you're not home, though it can be used for a much wider set of things.

This project is NOT in active development -- that ended in like 2009ish. I dug this up to post here by request. It contains a full implementation of the adaboost algorithm (which you can also find here: https://github.com/astromme/AdaBoost), and a GUI written in Qt to help train the model. 

The adaboost algorithm requires Boost (http://www.boost.org/), and the GUI obviously requires Qt (http://qt-project.org/).

In general, the workflow is as follows:

1. Get some images (see `images` directory).
2. Train a classifier (see `trainer` directory).
  1. Select a pair of images. This pair will be one training example.
  2. Decide whether the images are sufficiently "different" (if there are enough changes between the two, indicating some event has occurred).
  3. If the images are different, label it as a "positive" example, otherwise label it as "negative".
  4. Repeat until you have a whole bunch of training examples. At least 100, probably.
3. Test the classifier on new data (see `tester` directory).
  1. Load up some never-before-seen data, and use the tester UI to see how your classifier runs.
  2. If the classifier isn't accurate enough, train more examples.
