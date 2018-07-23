// Example usage for ParticleMQTTWrapper library by John Rodgers.

#include "ParticleMQTTWrapper.h"

// Initialize objects from the lib
ParticleMQTTWrapper particleMQTTWrapper;

void setup() {
    // Call functions on initialized library objects that require hardware
    particleMQTTWrapper.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    particleMQTTWrapper.process();
}
