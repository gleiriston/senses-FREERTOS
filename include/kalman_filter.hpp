#pragma once


class KalmanFilter {
public:
    KalmanFilter(float process_noise, float measurement_noise, float estimation_error, float initial_value) {
        this->q = process_noise;
        this->r = measurement_noise;
        this->p = estimation_error;
        this->x = initial_value;
    }

    float update(float measurement) {
        // Prediction update
        this->p = this->p + this->q;

        // Measurement update
        float k = this->p / (this->p + this->r);
        this->x = this->x + k * (measurement - this->x);
        this->p = (1 - k) * this->p;

        return this->x;
    }

private:
    float q; // Process noise covariance
    float r; // Measurement noise covariance
    float p; // Estimation error covariance
    float x; // Value
};



