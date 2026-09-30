#pragma once
#include <QPalette>
#include <QApplication> 

inline void setLightPalette() {
  
 QPalette lightPalette;

 // Base background colors
 lightPalette.setColor(QPalette::Window, QColor(240, 240, 240));
 lightPalette.setColor(QPalette::WindowText, QColor(0, 0, 0));
 lightPalette.setColor(QPalette::Base, QColor(255, 255, 255));
 lightPalette.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
 lightPalette.setColor(QPalette::ToolTipBase, QColor(255, 255, 220));
 lightPalette.setColor(QPalette::ToolTipText, QColor(0, 0, 0));
 lightPalette.setColor(QPalette::Text, QColor(0, 0, 0));
 
 // UI Elements (Buttons)
 lightPalette.setColor(QPalette::Button, QColor(240, 240, 240));
 lightPalette.setColor(QPalette::ButtonText, QColor(0, 0, 0));
 lightPalette.setColor(QPalette::BrightText, QColor(255, 255, 255));
 
 // Selection and Highlights
 lightPalette.setColor(QPalette::Highlight, QColor(0, 120, 215)); // Standard Windows Blue
 lightPalette.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
 
 // Disabled state color adjustments (Optional but recommended)
 lightPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(120, 120, 120));
 lightPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(120, 120, 120));
 lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(120, 120, 120));
 
 // Apply the palette to the entire application
 qApp->setPalette(lightPalette);
}
