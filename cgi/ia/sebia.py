from gradio_client import Client

client = Client("yeq6x/Image2Body_gradio")
result = client.predict(
		language="en",
		api_name="/update_language"
)
print(result)