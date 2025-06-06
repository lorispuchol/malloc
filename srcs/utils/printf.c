#include "printf.h"
#include "utils.h"

// Helper function to handle width and padding
static void print_with_width(const char *str, int width, int left_align, char pad_char) {
    int len = my_strlen(str);
    int padding = width - len;
    
    if (padding <= 0) {
        my_putstr(str);
        return;
    }
    
    if (!left_align) {
        // Right align - print padding first
        while (padding-- > 0) {
            my_putchar(pad_char);
        }
        my_putstr(str);
    } else {
        // Left align - print string first
        my_putstr(str);
        while (padding-- > 0) {
            my_putchar(' ');
        }
    }
}

// Helper function to convert number to string for width handling
static char* nbr_to_str(long long n, int base, int uppercase, int show_sign, int space_prefix) {
    static char buffer[65];
    char digits[] = "0123456789abcdef";
    char DIGITS[] = "0123456789ABCDEF";
    int i = 0;
    int negative = 0;
    
    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }
    
    if (n < 0 && base == 10) {
        negative = 1;
        n = -n;
    }
    
    while (n > 0) {
        buffer[i++] = uppercase ? DIGITS[n % base] : digits[n % base];
        n /= base;
    }
    
    if (negative) {
        buffer[i++] = '-';
    } else if (show_sign && base == 10) {
        buffer[i++] = '+';
    } else if (space_prefix && base == 10) {
        buffer[i++] = ' ';
    }
    
    buffer[i] = '\0';
    
    // Reverse the string
    int start = 0, end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    
    return buffer;
}

// Helper function to convert unsigned number to string for width handling
static char* unbr_to_str(unsigned long long n, int base, int uppercase, int alternate_form) {
    static char buffer[67]; // Extra space for 0x prefix
    char digits[] = "0123456789abcdef";
    char DIGITS[] = "0123456789ABCDEF";
    int i = 0;
    int start_pos = 0;
    
    if (n == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }
    
    // Add prefix for alternate form
    if (alternate_form && n != 0) {
        if (base == 16) {
            buffer[start_pos++] = '0';
            buffer[start_pos++] = uppercase ? 'X' : 'x';
        } else if (base == 8) {
            buffer[start_pos++] = '0';
        }
    }
    
    while (n > 0) {
        buffer[start_pos + i++] = uppercase ? DIGITS[n % base] : digits[n % base];
        n /= base;
    }
    
    buffer[start_pos + i] = '\0';
    
    // Reverse only the number part (not the prefix)
    int start = start_pos, end = start_pos + i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }
    
    return buffer;
}

// Main printf function
int my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    int count = 0;
    
    while (*format) {
        if (*format != '%') {
            my_putchar(*format);
            count++;
            format++;
            continue;
        }
        
        format++; // Skip '%'
        
        // Handle flags
        int left_align = 0;
        int show_sign = 0;
        int pad_zero = 0;
        int alternate_form = 0;
        int space_prefix = 0;
        
        while (*format) {
            if (*format == '-') left_align = 1;
            else if (*format == '+') show_sign = 1;
            else if (*format == '0') pad_zero = 1;
            else if (*format == '#') alternate_form = 1;
            else if (*format == ' ') space_prefix = 1;
            else break;
            format++;
        }
        
        // Handle width
        int width = 0;
        if (*format == '*') {
            width = va_arg(args, int);
            format++;
        } else {
            while (*format >= '0' && *format <= '9') {
                width = width * 10 + (*format - '0');
                format++;
            }
        }
        
        // Handle precision
        int precision = -1;
        if (*format == '.') {
            format++;
            precision = 0;
            if (*format == '*') {
                precision = va_arg(args, int);
                format++;
            } else {
                while (*format >= '0' && *format <= '9') {
                    precision = precision * 10 + (*format - '0');
                    format++;
                }
            }
        }
        
        // Handle length modifiers (simplified)
        int is_long = 0;
        int is_long_long = 0;
        if (*format == 'l') {
            is_long = 1;
            format++;
            if (*format == 'l') {
                is_long_long = 1;
                format++;
            }
        } else if (*format == 'h') {
            format++;
            if (*format == 'h') format++; // hh
        }
        
        // Handle conversion specifiers
        switch (*format) {
            case 'c': {
                char c = (char)va_arg(args, int);
                if (width > 1) {
                    if (left_align) {
                        my_putchar(c);
                        for (int i = 1; i < width; i++) my_putchar(' ');
                    } else {
                        for (int i = 1; i < width; i++) my_putchar(' ');
                        my_putchar(c);
                    }
                } else {
                    my_putchar(c);
                }
                count += width > 1 ? width : 1;
                break;
            }
            case 's': {
                char *str = va_arg(args, char*);
                if (!str) str = "(null)";
                
                int len = my_strlen(str);
                if (precision >= 0 && precision < len) {
                    // Print only precision characters
                    char temp_str[precision + 1];
                    for (int i = 0; i < precision; i++) {
                        temp_str[i] = str[i];
                    }
                    temp_str[precision] = '\0';
                    print_with_width(temp_str, width, left_align, ' ');
                    count += width > precision ? width : precision;
                } else {
                    print_with_width(str, width, left_align, ' ');
                    count += width > len ? width : len;
                }
                break;
            }
            case 'd':
            case 'i': {
                long long n;
                if (is_long_long) n = va_arg(args, long long);
                else if (is_long) n = va_arg(args, long);
                else n = va_arg(args, int);
                
                char *str = nbr_to_str(n, 10, 0, show_sign, space_prefix);
                char pad_char = (pad_zero && !left_align) ? '0' : ' ';
                print_with_width(str, width, left_align, pad_char);
                int len = my_strlen(str);
                count += width > len ? width : len;
                break;
            }
            case 'u': {
                unsigned long long n;
                if (is_long_long) n = va_arg(args, unsigned long long);
                else if (is_long) n = va_arg(args, unsigned long);
                else n = va_arg(args, unsigned int);
                
                char *str = unbr_to_str(n, 10, 0, alternate_form);
                char pad_char = (pad_zero && !left_align) ? '0' : ' ';
                print_with_width(str, width, left_align, pad_char);
                int len = my_strlen(str);
                count += width > len ? width : len;
                break;
            }
            case 'o': {
                unsigned long long n;
                if (is_long_long) n = va_arg(args, unsigned long long);
                else if (is_long) n = va_arg(args, unsigned long);
                else n = va_arg(args, unsigned int);
                
                char *str = unbr_to_str(n, 8, 0, alternate_form);
                char pad_char = (pad_zero && !left_align) ? '0' : ' ';
                print_with_width(str, width, left_align, pad_char);
                int len = my_strlen(str);
                count += width > len ? width : len;
                break;
            }
            case 'x': {
                unsigned long long n;
                if (is_long_long) n = va_arg(args, unsigned long long);
                else if (is_long) n = va_arg(args, unsigned long);
                else n = va_arg(args, unsigned int);
                
                char *str = unbr_to_str(n, 16, 0, alternate_form);
                char pad_char = (pad_zero && !left_align) ? '0' : ' ';
                print_with_width(str, width, left_align, pad_char);
                int len = my_strlen(str);
                count += width > len ? width : len;
                break;
            }
            case 'X': {
                unsigned long long n;
                if (is_long_long) n = va_arg(args, unsigned long long);
                else if (is_long) n = va_arg(args, unsigned long);
                else n = va_arg(args, unsigned int);
                
                char *str = unbr_to_str(n, 16, 1, alternate_form);
                char pad_char = (pad_zero && !left_align) ? '0' : ' ';
                print_with_width(str, width, left_align, pad_char);
                int len = my_strlen(str);
                count += width > len ? width : len;
                break;
            }
            case 'p': {
                void *ptr = va_arg(args, void*);
                if (ptr == 0) {
                    print_with_width("(nil)", width, left_align, ' ');
                    count += width > 5 ? width : 5;
                } else {
                    my_putstr("0x");
                    char *str = unbr_to_str((unsigned long long)(unsigned long)ptr, 16, 0, 0);
                    my_putstr(str);
                    count += 2 + my_strlen(str);
                }
                break;
            }
            case '%': {
                my_putchar('%');
                count++;
                break;
            }
            default: {
                // Unknown format specifier, just print it
                my_putchar('%');
                my_putchar(*format);
                count += 2;
                break;
            }
        }
        
        format++;
    }
    
    va_end(args);
    return count;
}