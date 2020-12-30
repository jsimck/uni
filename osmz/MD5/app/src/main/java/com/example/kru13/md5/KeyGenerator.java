package com.example.kru13.md5;

/**
 * Created by admin on 02.05.17.
 */

public class KeyGenerator {
    private static final char START = 65;
    private static final char END = 90;
    private static final char characters[] = {
        'A', 'B','C','D','E','F','G','H','I','J','K','L','M',
        'N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
    };

    private int i[];
    private int len;

    KeyGenerator() {
        len = 0;
        i = new int[512];

        for (int j = 0; j < 512; j++) {
            i[j] = 0;
        }
    }

    public String generate() {
        // Generate key
        StringBuilder sb = new StringBuilder();
        for (int l = 0; l <= len; l++) {
            sb.append(characters[i[l]]);
        }

        // Update indexes to the start
        for (int l = len; l >= 0; l--) {
            i[l]++;
            if (i[l] + START > END) {
                i[l] = 0;

                // End
                if (l == 0) {
                    len++;
                    break;
                }
            } else {
                break;
            }
        }

        return sb.toString();
    }
}
