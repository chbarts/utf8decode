`utf8decode`: A simple non-validating UTF-8 decoder and encoder
===============================================================

`utf8decode` contains functions to do two things: Turn sequences of bytes that can be interpreted as UTF-8 into 32-bit codepoint values, and turn 32-bit codepoint values into sequences of bytes that can be interpreted as UTF-8. No checking is done to validate whether an input UTF-8 byte sequence is overlong or whether the 32-bit value being input or output corresponds to an actual codepoint.
