/*
 * CompositeLogger.ino - Example demonstrating multi-destination logging
 *
 * This example shows how to use CompositeLogger to send log messages to multiple
 * destinations simultaneously. It demonstrates logger registration, different log
 * levels, and dynamic logger management.
 *
 * ## What This Example Does:
 *
 * 1. **Creates Multiple Loggers**: Sets up SerialLogger and demonstrates concepts
 * 2. **Registers Loggers**: Shows how to add multiple loggers to CompositeLogger
 * 3. **Multi-Destination Logging**: Demonstrates logging to multiple outputs
 * 4. **Dynamic Management**: Shows adding/removing loggers at runtime
 * 5. **Error Handling**: Demonstrates capacity limits and duplicate prevention
 *
 * ## Expected Output:
 *
 * All log messages will appear on Serial monitor, with additional messages showing
 * logger management operations. In a real application, you might see the same
 * messages appear in multiple destinations (Serial, SD card, network, etc.).
 *
 * ## Hardware Requirements:
 *
 * - Any Arduino-compatible board
 * - Serial monitor for viewing output
 * - Optional: Additional hardware for other logger types (SD card, WiFi, etc.)
 *
 * Authors: Marcel Erz (RetroStack)
 * Released under the MIT License.
 */

#include <Arduino.h>
#include <CompositeLogger.h>
#include <SerialLogger.h>

// Create logger instances
SerialLogger serialLogger1;
SerialLogger serialLogger2;  // Second serial logger for demonstration
CompositeLogger multiLogger;

// Simulate system components
int systemStatus = 0;
int errorCount = 0;
unsigned long lastLogTime = 0;

void setup()
{
    // Initialize serial communication
    Serial.begin(115200);
    delay(2000); // Allow serial to stabilize
    
    Serial.println("=== CompositeLogger Multi-Destination Logging Example ===");
    Serial.println();
    
    // Demonstrate basic logger registration
    Serial.println("📝 Setting up composite logger...");
    
    // Add first logger
    if (multiLogger.addLogger(&serialLogger1)) {
        Serial.println("✓ SerialLogger1 registered successfully");
    } else {
        Serial.println("✗ Failed to register SerialLogger1");
    }
    
    // Add second logger (in real application, this might be FileLogger, NetworkLogger, etc.)
    if (multiLogger.addLogger(&serialLogger2)) {
        Serial.println("✓ SerialLogger2 registered successfully");
    } else {
        Serial.println("✗ Failed to register SerialLogger2");
    }
    
    Serial.printf("📊 Total loggers registered: %d\n", multiLogger.getLoggerCount());
    Serial.println();
    
    // Test duplicate prevention
    Serial.println("🔒 Testing duplicate prevention...");
    if (multiLogger.addLogger(&serialLogger1)) {
        Serial.println("✗ Duplicate logger was added (this shouldn't happen!)");
    } else {
        Serial.println("✓ Duplicate logger correctly rejected");
    }
    Serial.println();
    
    // Now start using the composite logger for all subsequent logging
    multiLogger.info("🚀 System initialization starting...");
    multiLogger.info("⚙️  Composite logger active with %d destinations", multiLogger.getLoggerCount());
    
    // Demonstrate different log levels
    multiLogger.info("ℹ️  Information: System configuration loaded");
    multiLogger.warn("⚠️  Warning: Using demo configuration");
    multiLogger.err("❌ Error simulation: Sensor #3 not responding");
    
    // Demonstrate Print interface
    multiLogger.print("🔧 Raw output test: ");
    multiLogger.print("Value=");
    multiLogger.print(42);
    multiLogger.println(" (direct print calls)");
    
    multiLogger.info("✅ Initialization complete");
    Serial.println();
    
    Serial.println("🔄 Starting main loop with periodic logging...");
    Serial.println("    (Watch for runtime logger management demonstrations)");
    Serial.println();
}

void loop()
{
    // Simulate system activity with periodic logging
    unsigned long currentTime = millis();
    
    // Log status every 3 seconds
    if (currentTime - lastLogTime >= 3000) {
        lastLogTime = currentTime;
        systemStatus++;
        
        // Simulate different system states
        switch (systemStatus % 8) {
            case 0:
                multiLogger.info("📈 System running normally (uptime: %lu ms)", currentTime);
                break;
                
            case 1:
                multiLogger.info("🔄 Processing batch %d", systemStatus / 8 + 1);
                break;
                
            case 2:
                multiLogger.warn("📊 Memory usage: %d%% (simulated)", random(60, 85));
                break;
                
            case 3:
                // Demonstrate dynamic logger removal
                if (multiLogger.hasLogger(&serialLogger2)) {
                    Serial.println("\n🔧 Runtime demo: Removing SerialLogger2...");
                    if (multiLogger.removeLogger(&serialLogger2)) {
                        Serial.println("✓ SerialLogger2 removed successfully");
                        Serial.printf("📊 Active loggers: %d\n\n", multiLogger.getLoggerCount());
                    }
                }
                multiLogger.info("🔧 Logger configuration changed");
                break;
                
            case 4:
                errorCount++;
                multiLogger.err("💥 Simulated error #%d in module X", errorCount);
                break;
                
            case 5:
                multiLogger.info("🧹 Maintenance cycle completed");
                break;
                
            case 6:
                // Demonstrate re-adding a logger
                if (!multiLogger.hasLogger(&serialLogger2)) {
                    Serial.println("\n🔧 Runtime demo: Re-adding SerialLogger2...");
                    if (multiLogger.addLogger(&serialLogger2)) {
                        Serial.println("✓ SerialLogger2 re-added successfully");
                        Serial.printf("📊 Active loggers: %d\n\n", multiLogger.getLoggerCount());
                    }
                }
                multiLogger.info("🔄 Logger configuration restored");
                break;
                
            case 7:
                multiLogger.warn("🔋 Power management: Switching to low-power mode");
                break;
        }
    }
    
    // Simulate some work
    delay(100);
}

/*
 * EXAMPLE OUTPUT:
 * 
 * === CompositeLogger Multi-Destination Logging Example ===
 * 
 * 📝 Setting up composite logger...
 * ✓ SerialLogger1 registered successfully
 * ✓ SerialLogger2 registered successfully
 * 📊 Total loggers registered: 2
 * 
 * 🔒 Testing duplicate prevention...
 * ✓ Duplicate logger correctly rejected
 * 
 * [INFO] 🚀 System initialization starting...
 * [INFO] ⚙️  Composite logger active with 2 destinations
 * [INFO] ℹ️  Information: System configuration loaded
 * [WARN] ⚠️  Warning: Using demo configuration
 * [ERR ] ❌ Error simulation: Sensor #3 not responding
 * 🔧 Raw output test: Value=42 (direct print calls)
 * [INFO] ✅ Initialization complete
 * 
 * 🔄 Starting main loop with periodic logging...
 *     (Watch for runtime logger management demonstrations)
 * 
 * [INFO] 📈 System running normally (uptime: 3000 ms)
 * [INFO] 🔄 Processing batch 1
 * [WARN] 📊 Memory usage: 73% (simulated)
 * 
 * 🔧 Runtime demo: Removing SerialLogger2...
 * ✓ SerialLogger2 removed successfully
 * 📊 Active loggers: 1
 * 
 * [INFO] 🔧 Logger configuration changed
 * [ERR ] 💥 Simulated error #1 in module X
 * [INFO] 🧹 Maintenance cycle completed
 * 
 * 🔧 Runtime demo: Re-adding SerialLogger2...
 * ✓ SerialLogger2 re-added successfully
 * 📊 Active loggers: 2
 * 
 * [INFO] 🔄 Logger configuration restored
 * [WARN] 🔋 Power management: Switching to low-power mode
 * ...
 */
