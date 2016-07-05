/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.12
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package orbit.model;

public class ShipParams {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected ShipParams(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(ShipParams obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        OrbitSimulatorJNI.delete_ShipParams(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setShipEdgeLength(double value) {
    OrbitSimulatorJNI.ShipParams_shipEdgeLength_set(swigCPtr, this, value);
  }

  public double getShipEdgeLength() {
    return OrbitSimulatorJNI.ShipParams_shipEdgeLength_get(swigCPtr, this);
  }

  public void setShipMass(double value) {
    OrbitSimulatorJNI.ShipParams_shipMass_set(swigCPtr, this, value);
  }

  public double getShipMass() {
    return OrbitSimulatorJNI.ShipParams_shipMass_get(swigCPtr, this);
  }

  public void setFuelMass(double value) {
    OrbitSimulatorJNI.ShipParams_fuelMass_set(swigCPtr, this, value);
  }

  public double getFuelMass() {
    return OrbitSimulatorJNI.ShipParams_fuelMass_get(swigCPtr, this);
  }

  public void setMaxRotation(Rotation value) {
    OrbitSimulatorJNI.ShipParams_maxRotation_set(swigCPtr, this, Rotation.getCPtr(value), value);
  }

  public Rotation getMaxRotation() {
    long cPtr = OrbitSimulatorJNI.ShipParams_maxRotation_get(swigCPtr, this);
    return (cPtr == 0) ? null : new Rotation(cPtr, false);
  }

  public void setMaxFuelUsagePerSec(double value) {
    OrbitSimulatorJNI.ShipParams_maxFuelUsagePerSec_set(swigCPtr, this, value);
  }

  public double getMaxFuelUsagePerSec() {
    return OrbitSimulatorJNI.ShipParams_maxFuelUsagePerSec_get(swigCPtr, this);
  }

  public void setImpulsePerFuel(double value) {
    OrbitSimulatorJNI.ShipParams_impulsePerFuel_set(swigCPtr, this, value);
  }

  public double getImpulsePerFuel() {
    return OrbitSimulatorJNI.ShipParams_impulsePerFuel_get(swigCPtr, this);
  }

  public void setFlightPlan(vector_of_part_flight_plan value) {
    OrbitSimulatorJNI.ShipParams_flightPlan_set(swigCPtr, this, vector_of_part_flight_plan.getCPtr(value), value);
  }

  public vector_of_part_flight_plan getFlightPlan() {
    long cPtr = OrbitSimulatorJNI.ShipParams_flightPlan_get(swigCPtr, this);
    return (cPtr == 0) ? null : new vector_of_part_flight_plan(cPtr, false);
  }

  public void setMaxOverload(double value) {
    OrbitSimulatorJNI.ShipParams_maxOverload_set(swigCPtr, this, value);
  }

  public double getMaxOverload() {
    return OrbitSimulatorJNI.ShipParams_maxOverload_get(swigCPtr, this);
  }

  public void setMaxHeating(double value) {
    OrbitSimulatorJNI.ShipParams_maxHeating_set(swigCPtr, this, value);
  }

  public double getMaxHeating() {
    return OrbitSimulatorJNI.ShipParams_maxHeating_get(swigCPtr, this);
  }

  public ShipParams() {
    this(OrbitSimulatorJNI.new_ShipParams(), true);
  }

}
