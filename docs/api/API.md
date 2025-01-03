# API Reference

## Core Web Server API

### HTTP Server Interface
```c
int startServer(int port);
```
Starts the HTTP server on the specified port.

- **Parameters:**
  - `port`: Port number to listen on (1-65535)
- **Returns:**
  - 0 on success
  - -1 on error

### String Utilities
```c
char *toUpperCase(char *str);
```
Converts a string to uppercase.

- **Parameters:**
  - `str`: String to convert (ISO-8859-1 encoded)
- **Returns:**
  - Pointer to converted string
  - NULL on error

## HTML 4.01 Components

### Forms
```html
<form method="POST" action="/submit">
  <!-- Form content -->
</form>
```
Standard form component following HTML 4.01 Strict.

### Tables
```html
<table summary="Data presentation">
  <tr><th>Header</th></tr>
  <tr><td>Data</td></tr>
</table>
```
Data presentation component following HTML 4.01 Strict.

## CSS 2.1 Styles

### Layout
```css
.container {
  width: 100%;
  margin: 0 auto;
}
```
Basic responsive layout following CSS 2.1 specification.

## JavaScript ES3 Functions

### Event Handlers
```javascript
function handleClick(event) {
  // Event handling code
}
```
Standard event handling following ECMAScript 3 specification.

## Error Codes

| Code | Description           | HTTP Status |
|------|--------------------- |-------------|
| 0    | Success             | 200         |
| 1    | Invalid Parameters  | 400         |
| 2    | Authentication Error| 401         |
| 3    | Permission Denied   | 403         |
| 4    | Not Found          | 404         |
| 5    | Server Error       | 500         |

## Data Formats

### Request Format
```plaintext
GET /path HTTP/1.0
Host: example.com
Accept: text/html
Accept-Charset: ISO-8859-1
```

### Response Format
```plaintext
HTTP/1.0 200 OK
Content-Type: text/html; charset=ISO-8859-1
Content-Length: <length>

<html>
...
</html>
```
