# HTML 4.01 Strict Standards and Validation Rules

## Overview
This document outlines the HTML 4.01 Strict standards and validation rules for the web application. All HTML documents must comply with these standards to ensure compatibility and consistency.

## Requirements

### Document Type Declaration
- Must use HTML 4.01 Strict DOCTYPE
- Must be the first line of every HTML document
```html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
```

### Document Encoding
- All files must use ISO-8859-1 encoding
- Must include content-type meta tag:
```html
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
```

### Document Structure
Required elements in order:
```html
<html>
<head>
    <!-- Meta tags and title -->
</head>
<body>
    <!-- Content -->
</body>
</html>
```

### Meta Tags
Required meta tags:
```html
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<meta name="robots" content="index, follow">
<meta name="description" content="Page description">
```

### Element Rules
- All element names must be lowercase
- All attribute names must be lowercase
- All elements must be properly nested
- All elements must be properly closed
- Attribute values must be quoted

### Required Attributes
- `alt` for `<img>` elements
- `type` for `<script>` elements
- `type` for `<style>` elements
- `title` for the document

### Banned Elements
- Deprecated HTML elements
- Presentational elements
- Frame elements
- HTML5 elements

### Allowed Elements
Common elements include:
- Block elements: `<div>`, `<p>`, `<h1>`-`<h6>`, `<ul>`, `<ol>`, `<li>`, `<table>`, `<form>`
- Inline elements: `<span>`, `<a>`, `<img>`, `<input>`, `<strong>`, `<em>`
- Head elements: `<title>`, `<meta>`, `<style>`, `<script>`

### Tables
- Must include `<thead>`, `<tbody>`, and optional `<tfoot>`
- Must use `<th>` for header cells
- Must not use tables for layout

### Forms
- All form controls must have labels
- All form fields must have `name` attributes
- Submit buttons must have `value` attributes

### CSS and JavaScript
- Must be inline (no external files)
- Must include type attributes:
```html
<style type="text/css">
<script type="text/javascript">
```

## Examples

### Valid HTML Document Structure
```html
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
    <title>Page Title</title>
    <style type="text/css">
        /* CSS content */
    </style>
    <script type="text/javascript">
        "use strict";
        /* JavaScript content */
    </script>
</head>
<body>
    <h1>Main Heading</h1>
    <p>Paragraph content.</p>
    <div>
        <table>
            <thead>
                <tr><th>Header</th></tr>
            </thead>
            <tbody>
                <tr><td>Data</td></tr>
            </tbody>
        </table>
    </div>
</body>
</html>
```

### Invalid HTML Examples
```html
<!-- Invalid: Missing DOCTYPE -->
<html>
    <head><title>Invalid</title></head>
    <body></body>
</html>

<!-- Invalid: Wrong encoding -->
<meta charset="utf-8">

<!-- Invalid: Unclosed elements -->
<p>Unclosed paragraph
<div>Unclosed div

<!-- Invalid: HTML5 elements -->
<nav>
<header>
<footer>
<article>

<!-- Invalid: Deprecated elements -->
<font>
<center>
<strike>

<!-- Invalid: Unquoted attributes -->
<img src=image.gif>
<div class=main>
