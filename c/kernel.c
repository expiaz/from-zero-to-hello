void main() {
    char *video_mm = (char *) 0xb8000;
    *video_mm = 'X'; 
}