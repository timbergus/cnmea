#pragma once

#include <numbers>
#include <string_view>
#include <variant>

/**
 * @namespace cnmea::types
 * @brief Contains types, constants, and structures for parsing NMEA sentences.
 */
namespace cnmea::types {

/**
 * @defgroup Units Conversion and Constants
 * @brief Constants and units used in NMEA parsing.
 * @{
 */

/** @brief Units used to represent speed. */
enum class SpeedUnits {
  ms,    ///< Meters per second
  kmh,   ///< Kilometers per hour
  knots, ///< Nautical miles per hour
};

/** @brief Units used to represent distance. */
enum class DistanceUnits {
  m,  ///< Meters
  km, ///< Kilometers
  ft, ///< Feet
};

/** @brief Cardinal directions for latitude, longitude, and magnetic variation.
 */
enum class Direction {
  North, ///< Northern hemisphere
  South, ///< Southern hemisphere
  East,  ///< Eastern hemisphere
  West,  ///< Western hemisphere
};

/** @brief Conversion factor: knots to meters per second */
constexpr double KNTOMS{0.514444444};

/** @brief Conversion factor: knots to kilometers per hour */
constexpr double KNTOKMH{1.85};

/** @} */ // end of Units Conversion and Constants

/**
 * @defgroup Errors Parsing Errors
 * @brief Possible parsing errors when reading NMEA sentences.
 * @{
 */
enum class ParseError {
  InvalidDirection,         ///< Direction character was invalid
  InvalidFormat,            ///< Sentence format was invalid
  MissingFields,            ///< Required fields were missing
  UnknownError,             ///< Unknown parsing error
  UnsupportedType,          ///< Unsupported NMEA sentence type
  InvalidLatitude,          ///< Latitude value invalid
  InvalidLongitude,         ///< Longitude value invalid
  InvalidSpeed,             ///< Speed value invalid
  InvalidCourse,            ///< Course value invalid
  InvalidUTCDate,           ///< UTC date value invalid
  InvalidUTCTime,           ///< UTC time value invalid
  InvalidMagneticVariation, ///< Magnetic variation value invalid
  InvalidMode               ///< Mode value invalid
};
/** @} */ // end of Errors

/**
 * @defgroup Coordinates Latitude and Longitude
 * @brief Structures representing geographical coordinates.
 * @{
 */

/**
 * @brief Represents a course in degrees.
 * @example
 * Course c(90.0);
 * double deg = c.value_degrees(); // 90.0
 * double rad = c.value_radians(); // 1.5708
 */
struct Course {
private:
  double degrees; ///< Course in degrees

public:
  explicit Course(double degrees) : degrees(degrees) {}
  double value_degrees() const { return degrees; }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

/**
 * @brief Represents a geographic latitude.
 * @example
 * Latitude lat(40.7128, Direction::North);
 * double deg = lat.value_degrees(); // 40.7128
 * double rad = lat.value_radians(); // 0.710572
 */
struct Latitude {
private:
  double degrees;      ///< Latitude value
  Direction direction; ///< Direction (North/South)

public:
  Latitude(double degrees, Direction direction)
      : degrees(degrees), direction(direction) {}
  double get_degrees() const { return degrees; }
  Direction get_direction() const { return direction; }
  double value_degrees() const {
    return (direction == Direction::North ? degrees : -degrees);
  }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

/**
 * @brief Represents a geographic longitude.
 * @example
 * Longitude lon(74.0060, Direction::West);
 * double deg = lon.value_degrees(); // -74.0060
 * double rad = lon.value_radians(); // -1.29165
 */
struct Longitude {
private:
  double degrees;      ///< Longitude value
  Direction direction; ///< Direction (East/West)

public:
  Longitude(double degrees, Direction direction)
      : degrees(degrees), direction(direction) {}
  double get_degrees() const { return degrees; }
  Direction get_direction() const { return direction; }
  double value_degrees() const {
    return (direction == Direction::East ? degrees : -degrees);
  }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};

/** @} */ // end of Coordinates

/**
 * @defgroup SpeedData Speed Representation
 * @{
 * @example
 * Speed s(15.0, SpeedUnits::knots);
 * double value = s.get_value(); // 15.0
 * SpeedUnits unit = s.get_units(); // knots
 */
struct Speed {
private:
  double value;     ///< Numeric speed value
  SpeedUnits units; ///< Unit of speed

public:
  explicit Speed(double value, SpeedUnits units = SpeedUnits::knots)
      : value(value), units(units) {}
  double get_value() const { return value; }
  SpeedUnits get_units() const { return units; }
};
/** @} */

/**
 * @defgroup TimeDate UTC Time and Date
 * @{
 * @example
 * UTCTime t{"12", "34", "56"};
 * UTCDate d{"23", "08", "2025"};
 */
struct UTCTime {
  std::string_view hours;   ///< Hours component
  std::string_view minutes; ///< Minutes component
  std::string_view seconds; ///< Seconds component
};

struct UTCDate {
  std::string_view day;   ///< Day component
  std::string_view month; ///< Month component
  std::string_view year;  ///< Year component
};
/** @} */

/**
 * @defgroup SentenceTypes NMEA Sentence Types
 * @{
 */
enum class Type {
  GGA, ///< Global Positioning System Fix Data
  GLL, ///< Geographic Position – Latitude/Longitude
  GSA, ///< GPS DOP and Active Satellites
  GSV, ///< GPS DOP and Active Satellites
  RMC, ///< Recommended Minimum Specific GPS/Transit Data
  VTG, ///< Track Made Good and Ground Speed
  ZDA  ///< Time & Date
};
/** @} */

/**
 * @defgroup StatusData Status Indicators
 * @{
 */
enum class Status {
  Valid,   ///< Data is valid
  Invalid, ///< Data is invalid
};
/** @} */

/**
 * @defgroup ModeData Mode Indicators
 * @{
 */
enum class Mode {
  Autonomous,   ///< Autonomous mode
  Differential, ///< Differential GPS
  Estimated,    ///< Estimated (dead reckoning)
  ManualInput,  ///< Manual input
  Simulation,   ///< Simulation mode
  NotValid,     ///< Not valid
  Precise,      ///< Precise
  RTKFixed,     ///< RTK fixed
  RTKFloat,     ///< RTK float
  Uncalibrated  ///< Uncalibrated
};
/** @} */

/**
 * @defgroup FixQualityData GPS Fix Quality
 * @{
 */
enum class FixQuality {
  Invalid = 0,           ///< Invalid fix
  GPS = 1,               ///< GPS fix
  DGPS = 2,              ///< DGPS fix
  PPS = 3,               ///< PPS fix
  RealTimeKinematic = 4, ///< RTK fixed
  FloatRTK = 5,          ///< RTK float
  Estimated = 6,         ///< Estimated fix
  ManualInput = 7,       ///< Manual input
  Simulation = 8         ///< Simulation
};
/** @} */

/**
 * @defgroup MagneticVariationData Magnetic Variation
 * @{
 * @example
 * MagneticVariation mv(5.0, Direction::East);
 * double deg = mv.value_degrees(); // 5.0
 */
struct MagneticVariation {
private:
  double degrees;      ///< Magnetic variation in degrees
  Direction direction; ///< Direction of variation (East/West)

public:
  MagneticVariation(double degrees, Direction direction)
      : degrees(degrees), direction(direction) {}
  double get_degrees() const { return degrees; }
  Direction get_direction() const { return direction; }
  double value_degrees() const {
    return (direction == Direction::East ? degrees : -degrees);
  }
  double value_radians() const {
    return value_degrees() * std::numbers::pi / 180.0;
  }
};
/** @} */

/**
 * @defgroup AltitudeData Altitude and Geoid
 * @{
 * @example
 * Altitude alt(100.0, DistanceUnits::m);
 * double m = alt.value_meters(); // 100
 * double ft = alt.value_feet();  // 328.084
 */
struct Altitude {
private:
  double value;        ///< Altitude value
  DistanceUnits units; ///< Units

public:
  explicit Altitude(double value, DistanceUnits units = DistanceUnits::m)
      : value(value), units(units) {}
  double get_value() const { return value; }
  DistanceUnits get_units() const { return units; }
  double value_meters() const { return value; }
  double value_feet() const { return value * 3.28084; }
};

struct GeoidSeparation {
private:
  double value;        ///< Value of separation
  DistanceUnits units; ///< Units

public:
  explicit GeoidSeparation(double value, DistanceUnits units = DistanceUnits::m)
      : value(value), units(units) {}
  double get_value() const { return value; }
  DistanceUnits get_units() const { return units; }
  double value_meters() const { return value; }
  double value_feet() const { return value * 3.28084; }
};
/** @} */

/**
 * @defgroup DGPSData DGPS Age and Station
 * @{
 * @example
 * AgeOfDgps age(120);
 * DgpsStationId station(5);
 */
struct AgeOfDgps {
private:
  double seconds; ///< Age in seconds

public:
  explicit AgeOfDgps(double seconds) : seconds(seconds) {}
  double value_seconds() const { return seconds; }
  double value_minutes() const { return seconds / 60.0; }
};

struct DgpsStationId {
private:
  int id; ///< Station ID

public:
  explicit DgpsStationId(int id) : id(id) {}
  int value() const { return id; }
};
/** @} */

/// @brief Mode 1: Selection of fix type (manual/auto).
enum class SelectionMode {
  Manual,   ///< Manual 2D/3D fix selection.
  Automatic ///< Automatic 2D/3D fix selection.
};

/// @brief Mode 2: Fix type.
enum class FixType {
  None,  ///< No fix
  TwoD,  ///< 2D fix
  ThreeD ///< 3D fix
};

/**
 * @brief Represents the GPS DOP (Dilution of Precision) values.
 */
struct DOP {
  double pdop; ///< Position DOP
  double hdop; ///< Horizontal DOP
  double vdop; ///< Vertical DOP
};

/**
 * @brief Represents the satellite information in the GSV sentence.
 */
struct Satellite {
  int prn;          ///< Satellite PRN (Pseudo-Random Noise number)
  double elevation; ///< Elevation angle in degrees (0–90)
  double azimuth;   ///< Azimuth angle in degrees (0–359)
  double snr;       ///< Signal-to-Noise Ratio in dBHz (0–99)
};

/**
 * @brief Generic variant element type for parse results.
 */
using Element = std::variant<types::ParseError>;

} // namespace cnmea::types
