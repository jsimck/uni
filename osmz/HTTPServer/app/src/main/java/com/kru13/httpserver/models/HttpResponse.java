package com.kru13.httpserver.models;

import java.nio.ByteBuffer;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

public class HttpResponse {
    public static final String CONTENT_TYPE_HTML = "text/html";
    public static final String CONTENT_TYPE_JPEG = "image/jpeg";

    // Data
    private Object mBody;

    // HTTP Header params
    private Map<String, String> headers;
    private int code;
    private String message;

    public HttpResponse() {
        this.headers = new HashMap<String, String>();
        this.code = 200;
        this.message = "OK";
    }

    public HttpResponse(int code, String message) {
        this();
        this.code = code;
        this.message = message;
    }

    public HttpResponse(int code, String message, String contentType, int contentLength) {
        this(code, message);
        setContentLength(contentLength);
        setContentType(contentType);
    }

    public HttpResponse(int code, String message, String contentType, int contentLength, Object body) {
        this(code, message, contentType, contentLength);
        this.mBody = body;
    }

    public HttpResponse(Object body) {
        this(200, "OK", CONTENT_TYPE_HTML, ((String) body).length(), body);
    }

    public String headersToString() {
        StringBuilder builder = new StringBuilder();

        // Set current date, if not set
        if (headers.get("Date") == null) {
            setDate(new Date());
        }

        // First line http mRequest
        builder.append(HttpRequest.HTTP_10).append(" ");
        builder.append(code).append(" ");
        builder.append(message).append("\n");

        // Parse headers
        for (Object o : headers.entrySet()) {
            Map.Entry e = (Map.Entry) o;
            builder.append(e.getKey()).append(": ").append(e.getValue()).append("\n");
        }

        builder.append("\n");
        return builder.toString();
    }

    public boolean isBinary() {
        return (mBody != null && mBody instanceof ByteBuffer);
    }

    public void setContentLength(int length) { setHeader("Content-Length", Integer.toString(length)); }
    public void setContentType(String type) {
        setHeader("Content-Type", type);
    }
    public void setDate(Date date) {
        setHeader("Date", date.toString());
    }
    public void setHeader(String key, String value) {
        headers.put(key, value);
    }

    public Object getBody() {
        return mBody;
    }
    public void setBody(Object body) { this.mBody = body; }
}
