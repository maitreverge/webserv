import sys

def main():
    try:
        # Lire et afficher le contenu lu depuis stdin sur stderr
        while True:
            buff = sys.stdin.read(20)
            if not buff:
                break
            sys.stderr.write(buff)
    except Exception as e:
        print("error read", file=sys.stdout)

    # Contenu HTML à envoyer
    body = """<html>
<head><title>My Styled Page</title></head>
<body style="background-color: #f0f0f0; text-align: center; padding: 50px;">
<h1 style="color: #ff5733; font-family: Arial, sans-serif;">Hello, Python!</h1>
<p style="color: #555; font-size: 18px;">This is a simple page with inline CSS.</p>
</body>
</html>"""

    # Construire l'en-tête HTTP
    header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
    length = len(body)
    end = "\r\n\r\n"

    # Afficher la réponse HTTP complète
    sys.stdout.write(f"{header}{length}{end}{body}")

if __name__ == "__main__":
    main()
