#pragma once

#include <frc/Encoder.h>
#include <units/angle.h>
#include <units/angular_velocity.h>

#include <rev/CANSparkMax.h>
#include <ctre/phoenix.h>

#include "Util.h"

namespace wom {
  class Encoder {
   public:
    Encoder(double encoderTicksPerRotation, double reduction) : _encoderTicksPerRotation(encoderTicksPerRotation), _reduction(reduction) {};
    virtual double    GetEncoderRawTicks() const = 0;
    virtual double    GetEncoderTickVelocity() const = 0;  // ticks/s
    virtual void      ZeroEncoder();

    double  GetEncoderTicks() const;
    double  GetEncoderTicksPerRotation() const;

    void SetReduction(double reduction);

    units::radian_t GetEncoderPosition();
    units::radians_per_second_t GetEncoderAngularVelocity();   // rad/s

   private:
    double _encoderTicksPerRotation;
    double _reduction = 1.0;
    double _offset = 0;
  };

  class DigitalEncoder : public Encoder {
   public:
    DigitalEncoder(int channelA, int channelB, double ticksPerRotation, double reduction = 1)
        : Encoder(ticksPerRotation, reduction),
          _nativeEncoder(channelA, channelB){};

    double GetEncoderRawTicks() const override;
    double GetEncoderTickVelocity() const override;

   private:
    frc::Encoder _nativeEncoder;
  };

  class CANSparkMaxEncoder : public Encoder {
   public:
    CANSparkMaxEncoder(rev::CANSparkMax *controller, double reduction = 1);

    double GetEncoderRawTicks() const override;
    double GetEncoderTickVelocity() const override;
   private:
    rev::SparkMaxRelativeEncoder _encoder;
  };

  class TalonFXEncoder : public Encoder {
   public:
    TalonFXEncoder(ctre::phoenix::motorcontrol::can::TalonFX *controller, double reduction = 1);

    double GetEncoderRawTicks() const override;
    double GetEncoderTickVelocity() const override;
   private:
    ctre::phoenix::motorcontrol::can::TalonFX *_controller;
  };
}  // namespace wom
