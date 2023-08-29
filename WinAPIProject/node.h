#pragma once
#include "struct.h"

class Node {
public:
    int x, y;
    float       fCost;      // F Value
    float       fHeuristic; // G + H Value∏¶ ∞ËªÍ«— »ﬁ∏ÆΩ∫∆Ω
    Node* pParent;

    Node(int _x, int _y, float cost = 0.f, Node* parent = nullptr)
        : x(_x), y(_y), fCost(cost), fHeuristic(0), pParent(parent) {}
    Node(float _x, float _y, float cost = 0.f, Node* parent = nullptr)
        : x((int)_x), y((int)_y), fCost(cost), fHeuristic(0), pParent(parent) {}
    Node(Vec2 _vPos, float cost = 0.f, Node* parent = nullptr)
        : x((int)_vPos.x), y((int)_vPos.y), fCost(cost), fHeuristic(0), pParent(parent) {}

    static float Heuristic(const Vec2& a, const Vec2& b)
    {
        return abs(a.x - b.x) + abs(a.y - b.y);
    }
};