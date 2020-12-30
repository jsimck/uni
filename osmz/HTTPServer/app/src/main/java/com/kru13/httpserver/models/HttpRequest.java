package com.kru13.httpserver.models;

import android.util.Log;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by jansimecek on 13.02.17.
 */

public class HttpRequest {
    private static final String TAG = "HttpRequest";

    // Helpers
    public static final String HTTP_10 = "HTTP/1.0";
    public static final String HTTP_11 = "HTTP/1.1";

    // HTML Request header params
    private String content;
    private String method;
    private String path;
    private String version;
    private String host;
    private Map<String, String> headers;

    private InputStream stream;

    public HttpRequest(InputStream stream) {
        this.stream = stream;
        parse();
    }

    public void parse() {
        try {
            BufferedReader in = new BufferedReader(new InputStreamReader(stream));

            // Save mRequest header to string which is then being parsed
            String line;
            StringBuilder buffer = new StringBuilder();

            // Read mRequest header
            while (!(line = in.readLine()).isEmpty()) {
                buffer.append(line);
                buffer.append("\n");
            }

            // Parse content
            content = buffer.toString();
        } catch (Exception e) {
            Log.d(TAG, "parse: " + e.getMessage());
            e.printStackTrace();
        }

        if (content.isEmpty()) {
            return;
        }

        // Split mRequest by lines
        String requestLines[] = content.split("\\n");
        headers = new HashMap<String, String>();

        // Parse method, path, version to separate variables
        parseFirstLine(requestLines[0]);

        // Parse other attributes
        for (int i = 1; i < requestLines.length - 1; i++) {
            Pattern p = Pattern.compile("^([\\w-]+)\\: (.*)$");
            Matcher m = p.matcher(requestLines[i]);
            m.find();

            // Put all header fields to hash map
            headers.put(m.group(1), m.group(2));

            // Save host to separate variable too
            if (m.group(1).equals("Host")) {
                host = m.group(2);
            }
        }
    }

    private void parseFirstLine(String input) {
        Pattern p = Pattern.compile("^(\\w+) (\\/.*) (HTTP\\/[12]\\.[01])$");
        Matcher m = p.matcher(input);
        m.find();

        // Save each group to its separate variable
        method = m.group(1);
        path = m.group(2);
        version = m.group(3);
    }

    @Override
    public String toString() {
        StringBuilder builder = new StringBuilder();

        // First line
        builder.append(method).append(" ").append(path).append(" ").append(version).append("\n");

        // Rest of fields
        for (Object o: headers.entrySet()) {
            Map.Entry entry = (Map.Entry) o;
            builder.append(entry.getKey()).append(": ").append(entry.getValue()).append("\n");
        }

        return builder.toString();
    }

    public String getContent() {
        return content;
    }
    public String getMethod() {
        return (method == null) ? "" : method;
    }
    public String getPath() {
        return (path == null) ? "" : path;
    }
    public String getVersion() {
        return (version == null) ? "" : version;
    }
    public String getHost() {
        return (host == null) ? "" : host;
    }
    public String getField(String field) { return headers.get(field); }
}
