#pragma once
#include <stdint.h>

/* PMD record types */
const uint16_t PAGE = 0x0500;
const uint16_t GLOBAL_INFO = 0x1800;
const uint16_t SHAPE = 0x1900;
const uint16_t LINE_SET = 0x1100;
const uint16_t XFORM = 0x2800;

/* Shape record types */
const uint8_t POLYGON_RECORD = 0x0C;
const uint8_t RECTANGLE_RECORD = 0x04;
const uint8_t ELLIPSE_RECORD = 0x05;

/* Polygon flags */
const uint8_t POLYGON_CLOSED = 0x03;
const uint8_t POLYGON_OPEN = 0x01;

/* Various constants */
const uint16_t ENDIANNESS_MARKER = 0x99FF;
const uint16_t WARPED_ENDIANNESS_MARKER = 0xFF99;

/* vim:set shiftwidth=2 softtabstop=2 expandtab: */
