exports.handler = async (event) => {
    try {
      // if (event.httpMethod !== "POST") {
      //   return {
      //     statusCode: 405,
      //     body: JSON.stringify({ error: "Method Not Allowed" }),
      //   };
      // }
  
      // Process the webhook payload
      const payload = JSON.parse(event.body);
  
      // Do something with the payload
      console.log("Webhook received:", payload);
  
      // Return a response (optional)
      return {
        statusCode: 200,
        body: JSON.stringify({ message: "Webhook received successfully!" }),
      };
    } catch (error) {
      console.error("Webhook error:", error);
      return {
        statusCode: 500,
        body: JSON.stringify({
          error: "An error occurred while processing the webhook.",
        }),
      };
    }
  };

  