// MotionTypes.h
#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

// Position structure to hold axis coordinates
struct PositionStruct {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double u = 0.0;
    double v = 0.0;
    double w = 0.0;
};

// Motion device representation
struct MotionDevice {
    bool IsEnabled = false;
    std::string IpAddress;
    int Port = 0;
    int Id = 0;
    std::string Name;
    std::map<std::string, PositionStruct> Positions;
    // New fields for enhanced configuration
    std::string TypeController = "PI";  // Default to PI if not specified
    std::string InstalledAxes = "XYZUVW";  // Default to all axes if not specified
};

// Graph node structure
struct Node {
    std::string Id;
    std::string Label;
    std::string Device;
    std::string Position;
    int X = 0;
    int Y = 0;
};

// Edge conditions for graph transitions
struct EdgeConditions {
    bool RequiresOperatorApproval = false;
    int TimeoutSeconds = 0;
    bool IsBidirectional = false;  // Add this new property
};

// Graph edge structure
struct Edge {
    std::string Id;
    std::string Source;
    std::string Target;
    std::string Label;
    EdgeConditions Conditions;
};

// Complete graph structure
struct Graph {
    std::vector<Node> Nodes;
    std::vector<Edge> Edges;
};

// Global settings
struct Settings {
    double DefaultSpeed = 10.0;
    double DefaultAcceleration = 5.0;
    std::string LogLevel = "info";
    bool AutoReconnect = true;
    int ConnectionTimeout = 5000;
    double PositionTolerance = 0.001;
};