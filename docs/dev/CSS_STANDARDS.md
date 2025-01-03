# CSS2.1 Standards and Validation Rules

## Overview
This document outlines the CSS2.1 standards and validation rules for HTML documents in the web application.

## Requirements

### Document Encoding
- All files must use ISO-8859-1 encoding
- Meta charset must be specified: `<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">`

### CSS Standards
- Must conform to CSS2.1 specification
- No CSS3 or modern features
- All styles must be inline (no external stylesheets)
- No vendor prefixes
- No @import rules

### Allowed Selectors
- Element selectors (e.g., `div`, `p`, `table`)
- Class selectors (e.g., `.header`, `.nav`)
- ID selectors (e.g., `#main`, `#footer`)
- Descendant selectors (e.g., `div p`, `.nav li`)
- Child selectors (e.g., `ul > li`)
- Adjacent sibling selectors (e.g., `h1 + p`)
- Attribute selectors (e.g., `input[type="text"]`)
- Pseudo-classes:
  - :hover
  - :active
  - :focus
  - :link
  - :visited
  - :first-child

### Disallowed Features
- CSS3 selectors
- Media queries (except `print`)
- Flexbox/Grid
- Transitions/Animations
- Custom properties (variables)
- calc() function
- Modern color formats (rgba, hsla)
- Border-radius
- Box-shadow
- Text-shadow
- Transform
- Multiple backgrounds
- Gradients
- Web fonts (@font-face)

### Value Types
- Colors: #hex, rgb(), named colors
- Units: px, em, %, pt
- Keywords: auto, none, inherit

### Properties
Limited to CSS2.1 specification properties including:
- margin, padding
- border
- background
- color
- font
- text-align
- display
- position
- width, height
- float, clear
- overflow
- visibility
- z-index

### Validation Rules
1. Style tags must include type attribute: `<style type="text/css">`
2. No external stylesheets
3. No @import rules
4. No CSS3 properties or selectors
5. ISO-8859-1 character encoding
6. Valid CSS2.1 syntax
7. No vendor prefixes
8. No modern features

## Examples

### Valid CSS
```css
/* Basic styling */
body {
    font-family: Arial, sans-serif;
    margin: 0;
    padding: 0;
    background-color: #ffffff;
}

/* Layout */
#header {
    width: 100%;
    height: 60px;
    background-color: #000000;
}

/* Typography */
.title {
    font-size: 16pt;
    color: #000000;
}

/* Tables */
table {
    border-collapse: collapse;
    width: 100%;
}

td, th {
    border: 1px solid #000000;
    padding: 4px;
}

/* Forms */
input[type="text"] {
    border: 1px solid #cccccc;
    padding: 2px;
}

/* Links */
a:hover {
    color: #ff0000;
}
```

### Invalid CSS
```css
/* Modern features - NOT allowed */
.container {
    display: flex;
    border-radius: 5px;
    box-shadow: 2px 2px 5px rgba(0,0,0,0.3);
    background: linear-gradient(to right, #fff, #eee);
}

@media screen and (max-width: 768px) {
    .container {
        flex-direction: column;
    }
}
