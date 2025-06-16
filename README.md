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

How It Works
The library precomputes trigonometric values at initialization and stores them in static lookup tables:

Direct Functions (sin/cos/tan): Uses a single sine table with phase shifting for cosine
Inverse Functions (asin/acos/atan): Separate tables with linear interpolation
Index Masking: Handles angle wrapping efficiently using bitwise operations
Cardinal Values: Ensures exact values for common angles (0°, 90°, 180°, 270°)

Performance Characteristics

Speed: 1.5 - 2.8x faster than std::sin/cos (depending on platform and compiler)
Memory Usage:

Default configuration: ~256KB for all tables combined
Configurable from 32KB to several MB


Accuracy: ~0.001 absolute error with default 16-bit tables
Initialization: One-time cost at program startup

Use Cases
Ideal for:

Real-time 3D graphics and transformations
Game physics and animation systems
Digital signal processing and audio synthesis
Robotics and control systems
Particle systems and procedural generation
Any application with millions of trig calls per second

Not recommended for:

Scientific computing requiring high precision
Applications with strict accuracy requirements
One-off calculations where memory usage matters more than speed
