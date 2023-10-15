#include <iostream>
#include "td3.hpp"
#include "support.hpp"
#include <stdlib.h>
#include <math.h>       // sin, cos
#include <assert.h>

using namespace std;

using namespace support;

double* extend_array(double* array, int length, int new_size) {
  double* narray = new double[new_size];
  for (int i = 0; i < length; i++) {
      narray[i] = array[i];
  }
  for (int i = length; i < new_size; i++) {
      narray[i] = 0;
  }
  delete[] array;
  return narray;
}

double* shrink_array(double* array, int length, int new_size) {
    double* narray = new double[new_size];
    for (int i = 0; i < new_size; i++) {
        narray[i] = array[i];
    }
    delete[] array;
    return narray;
}

double* append_to_array(double element,
                        double* array,
                        int &current_size,
                        int &max_size) {
  if  (current_size == max_size) {
      array = extend_array(array, max_size, max_size + 5);
      array[current_size] = element;
      max_size+=5;
      current_size++;
  }
  else {
      array[current_size] = element;
      current_size++;
  }
  return array; // YOU CAN CHANGE THIS
}

double* remove_from_array(double* array,
                          int &current_size,
                          int &max_size) {
  array[current_size - 1] = 0;
  current_size--;
  if (max_size - current_size >= 5) {
      array = shrink_array(array, max_size, current_size);
      max_size = current_size;
  }
  return array;
}

bool simulate_projectile(const double magnitude, const double angle,
                         const double simulation_interval,
                         double *targets, int &tot_targets,
                         int *obstacles, int tot_obstacles,
                         double* &telemetry,
                         int &telemetry_current_size,
                         int &telemetry_max_size) {
  // YOU CAN MODIFY THIS FUNCTION TO RECORD THE TELEMETRY

  bool hit_target, hit_obstacle;
  double v0_x, v0_y, x, y, t;
  double PI = 3.14159265;
  double g = 9.8;

  v0_x = magnitude * cos(angle * PI / 180);
  v0_y = magnitude * sin(angle * PI / 180);

  t = 0;
  x = 0;
  y = 0;

  hit_target = false;
  hit_obstacle = false;
  while (y >= 0 && (! hit_target) && (! hit_obstacle)) {
    double * target_coordinates = find_collision(x, y, targets, tot_targets);
    if (target_coordinates != NULL) {
      remove_target(targets, tot_targets, target_coordinates);
      hit_target = true;
    } else if (find_collision(x, y, obstacles, tot_obstacles) != NULL) {
      hit_obstacle = true;
    } else {
      t = t + simulation_interval;
      y = v0_y * t  - 0.5 * g * t * t;
      x = v0_x * t;
    }
    telemetry = append_to_array(t, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(x, telemetry, telemetry_current_size, telemetry_max_size);
    telemetry = append_to_array(y, telemetry, telemetry_current_size, telemetry_max_size);

  }

  return hit_target;
}

void part(double *ar, int b, int e, int& p) {
    int i = b;
    double tmp1, tmp2, tmp3;
    while (i < e) {
        if (ar[3*i] <= ar[3*e]) {
            tmp1 = ar[3*b];
            tmp2 = ar[3*b+1];
            tmp3 = ar[3*b+2];
            ar[3*b] = ar[3*i];
            ar[3*b+1] = ar[3*i+1];
            ar[3*b+2] = ar[3*i+2];
            ar[3*i] = tmp1;
            ar[3*i+1] = tmp2;
            ar[3*i+2] = tmp3;
            b++;
        }
        i++;
    }
    tmp1 = ar[3*b];
    tmp2 = ar[3*b+1];
    tmp3 = ar[3*b+2];
    ar[3*b] = ar[3*e];
    ar[3*b+1] = ar[3*e+1];
    ar[3*b+2] = ar[3*e+2];
    ar[3*e] = tmp1;
    ar[3*e+1] = tmp2;
    ar[3*e+2] = tmp3;
    p = b;
}
void qsort(double *ar, int b, int e) {
    if (b >= e) return;
    int p;
    part(ar, b, e, p);
    qsort(ar, b, p-1);
    qsort(ar, p+1, e);
}
void merge_telemetry(double **telemetries,
                     int tot_telemetries,
                     int *telemetries_sizes,
                     double* &global_telemetry,
                     int &global_telemetry_current_size,
                     int &global_telemetry_max_size) {
 for (int i = 0; i < tot_telemetries; i++) {
     for (int j = 0; j < telemetries_sizes[i]; j++) {
         global_telemetry = append_to_array(telemetries[i][j], global_telemetry, global_telemetry_current_size, global_telemetry_max_size);
     }
 }
 qsort(global_telemetry, 0, global_telemetry_current_size/3 - 1);
}
