exports.handler = async (event) => {
  try {
      // Process the webhook payload (e.g., parse JSON)
      const payload = JSON.parse(event.body);
      console.log("HIIIII");
      // Process the data as needed
      // Example: Extract relevant information from the payload
      // const processedData = {
      //     temperature: payload.temperature,
      //     humidity: payload.humidity,
      //     // Add more processing logic as necessary
      // };

      // Return the processed data to the frontend
      return {
          statusCode: 200,
          body: JSON.stringify(payload)
      };
  } catch (error) {
      console.error("Webhook error:", error);
      return {
          statusCode: 500,
          body: JSON.stringify({
              error: "An error occurred while processing the webhook."
          })
      };
  }
};
