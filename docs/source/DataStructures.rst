********************
2.2. Data Structures
********************
-------------------------------------------------------------------------------------------------------------------------------------------

2.2.1. hcrngStreamState
^^^^^^^^^^^^^^^^^^^^^^^

::

 struct hcrngStreamState

Stream state [host/device]. Contains the state of a random stream. The definition of a state depends on the type of generator.

Examples:
    `Multistream.cpp <Multistream.cpp.html>`_.

2.2.2. hcrngStream
^^^^^^^^^^^^^^^^^^

::

 struct hcrngStream

Stream object [host/device]. A structure that contains the current information on a stream object. It generally depends on the type of generator. It typically stores the current state, the initial state of the stream, and the initial state of the current substream.

Examples:
    `Multistream.cpp <Multistream.cpp.html>`_, and `RandomArray.cpp <Randomarray.cpp.html>`_.

2.2.3. hcrngStreamCreator
^^^^^^^^^^^^^^^^^^^^^^^^^

::

 struct hcrngStreamCreator

Stream creator object. For each type of RNG, there is a single default creator of streams, and this should be sufficient for most applications. Multiple creators could be useful for example to create the same successive stream objects multiple times in the same order, instead of storing them in an array and reusing them, or to create copies of the same streams in the same order at different locations in a distributed system, e.g., when simulating similar systems with common random numbers. Stream creators are created according to an abstract factory pattern.
