Fast Trigonometric Functions Library
A high-performance C++ template library that provides fast trigonometric and inverse trigonometric functions using precomputed lookup tables (LUTs) with linear interpolation.
Overview
This library trades memory for speed by replacing expensive trigonometric calculations with table lookups. It's designed for applications where trigonometric performance is critical and approximate results are acceptable, such as real-time graphics, game engines, audio processing, and embedded systems.
Key Features

Fast Evaluation: Orders of magnitude faster than standard library functions
Template-Based Design: Fully customizable precision (float/double) and table sizes
Comprehensive Coverage: Includes sin, cos, tan, csc, sec, cot, asin, acos, and atan
Memory Efficient: Configurable table sizes to balance accuracy vs memory usage
Header-Only: Simple integration with no linking required
Modern C++: Uses C++17 features for type safety and performance
Thread-Safe: Read-only lookup tables initialized at program startup
