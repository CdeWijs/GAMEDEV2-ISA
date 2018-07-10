// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup() {
	GetMesh()->SetSimulatePhysics(true);

	// The base power level of the battery
	BatteryPower = 150.0f;
}

void ABatteryPickup::WasCollected_Implementation() {
	// Use the base pickup behaviour
	Super::WasCollected_Implementation();
	//Destroy the battery
	Destroy();
}

// Returns the power level of the battery
float ABatteryPickup::GetBatteryPower() {
	return BatteryPower;
}


