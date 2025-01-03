# ECMAScript 3 Standards and Validation Rules

## Overview
This document outlines the ECMAScript 3 (ES3) standards and validation rules for HTML documents in the web application.

## Requirements

### Document Encoding
- All files must use ISO-8859-1 encoding
- Scripts must be inline (no external JavaScript files)
- Script tags must include type attribute: `<script type="text/javascript">`

### JavaScript Standards
- Must conform to ECMAScript 3 specification
- No modern JavaScript features (ES5+)
- Must use "use strict" directive
- All scripts must be inline
- No external libraries or frameworks

### Allowed Features
- Basic data types (Number, String, Boolean, Object, Array)
- Control structures (if, for, while, switch)
- Functions (function declarations and expressions)
- Objects and prototypes
- Basic DOM manipulation (getElementById, getElementsByTagName)
- Core JavaScript objects (Date, Math, RegExp)
- Basic event handling (onclick, onload)
- Error handling (try/catch)
- Standard browser objects (window, document, history)

### Disallowed Features
- let/const declarations
- Arrow functions
- Template literals
- Destructuring
- Default parameters
- Rest/Spread operators
- Classes
- Modules
- Promises
- async/await
- Map/Set/WeakMap/WeakSet
- Array methods (forEach, map, filter, etc.)
- String methods (includes, startsWith, etc.)
- Object methods (keys, values, entries)
- Modern DOM APIs

### Variable Declaration
- Must use var for variable declarations
- Variables must be declared before use
- Global variables should be minimized

### Function Guidelines
- Use function declarations or expressions
- No arrow functions
- No method shorthand
- No default parameters

### DOM Manipulation
Use basic DOM methods:
- document.getElementById()
- document.getElementsByTagName()
- document.getElementsByName()
- element.getAttribute()
- element.setAttribute()

### Event Handling
Use traditional event handling:
```javascript
element.onclick = function() {
    // handler code
};
// or
function handleClick(event) {
    // handler code
}
element.onclick = handleClick;
```

### Error Handling
Use traditional try-catch:
```javascript
try {
    // code that may throw error
} catch (error) {
    // handle error
}
```

## Examples

### Valid JavaScript
```javascript
"use strict";

// Variable declarations
var count = 0;
var name = "John";
var isValid = true;

// Functions
function calculateTotal(price, quantity) {
    return price * quantity;
}

// Object creation
var person = {
    name: "John",
    age: 30,
    sayHello: function() {
        return "Hello, " + this.name;
    }
};

// DOM manipulation
var element = document.getElementById("myElement");
if (element) {
    element.style.display = "none";
}

// Event handling
function handleClick(event) {
    alert("Clicked!");
}
element.onclick = handleClick;

// Error handling
try {
    // risky code
} catch (error) {
    alert("An error occurred: " + error.message);
}
```

### Invalid JavaScript
```javascript
// Modern features - NOT allowed
const name = "John";
let age = 30;

// Arrow function - NOT allowed
const greet = () => {
    console.log("Hello");
};

// Template literals - NOT allowed
const message = `Hello ${name}`;

// Destructuring - NOT allowed
const { firstName, lastName } = person;

// Modern array methods - NOT allowed
array.forEach(item => {
    console.log(item);
});

// Classes - NOT allowed
class Person {
    constructor(name) {
        this.name = name;
    }
}

// Promises - NOT allowed
fetch("/api/data")
    .then(response => response.json())
    .then(data => console.log(data));

// async/await - NOT allowed
async function getData() {
    const response = await fetch("/api/data");
    const data = await response.json();
    return data;
}
