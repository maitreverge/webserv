import sys
import webbrowser
import re

def get_urls_from_file(file_path):
    urls = []
    with open(file_path, 'r') as file:
        for line in file:
            match = re.search(r'(http[s]?://[^\s]+)', line)
            if match:
                urls.append(match.group(0))
    return urls

if len(sys.argv) != 2:
    print("Usage: python3 launch_chrome.py <config_number>")
    sys.exit(1)

config_number = sys.argv[1]
config_file_path = f'Requests_Tester/requests/config_{config_number}_requests.txt'

urls = get_urls_from_file(config_file_path)

if not urls:
    print(f"No URLs found in {config_file_path}")
    sys.exit(1)

# Path to Google Chrome command
chrome_command = 'google-chrome'

# Register the browser
webbrowser.register('chrome', None, webbrowser.BackgroundBrowser(chrome_command))

# Open all URLs in new tabs
for url in urls:
    webbrowser.get('chrome').open_new_tab(url)