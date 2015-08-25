package io.botan.sdk;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.core.type.TypeReference;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpResponseException;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.utils.URIBuilder;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.entity.ContentType;
import org.apache.http.entity.StringEntity;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.nio.client.HttpAsyncClient;
import org.apache.http.util.EntityUtils;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.Map;
import java.util.concurrent.CompletableFuture;

/**
 * Simple asynchronous client implementation for {@linkplain Botan https://botan.io/}.
 *
 * @author <a href="mailto:ketoth.xupack@gmail.com">Ketoth Xupack</a>
 * @since 2015-08-24 23:23
 */
@SuppressWarnings("EmptyClass")
public final class Botan {
    private static final TypeReference<Map<String, String>> MAP_TYPE = new TypeReference<Map<String, String>>() {};

    private final HttpAsyncClient client;
    private final ObjectMapper mapper;

    public Botan(final HttpAsyncClient client, final ObjectMapper mapper) {
        this.client = client;
        this.mapper = mapper;
    }

    public CompletableFuture<?> track(final String token,
                                      final String uid,
                                      final Object message,
                                      final String name) {
        CompletableFuture<Void> future = new CompletableFuture<>();

        final String json;
        try {
            json = mapper.writeValueAsString(message);
        } catch (JsonProcessingException e) {
            future.completeExceptionally(e);
            return future;
        }

        final URIBuilder uriBuilder = new URIBuilder()
                .setScheme("https")
                .setHost("api.botan.io")
                .setPath("/track")
                .setParameters(
                        new BasicNameValuePair("token", token),
                        new BasicNameValuePair("uid", uid),
                        new BasicNameValuePair("name", name)
                );
        final URI uri;
        try {
            uri = uriBuilder.build();
        } catch (URISyntaxException e) {
            throw new IllegalStateException(e);
        }

        final HttpPost request = new HttpPost(uri);
        request.setEntity(new StringEntity(json, ContentType.APPLICATION_JSON));

        client.execute(request, new FutureCallback<HttpResponse>() {
            @Override
            public void completed(final HttpResponse response) {
                int statusCode = response.getStatusLine().getStatusCode();
                if (statusCode != HttpStatus.SC_OK) {
                    String reasonPhrase = response.getStatusLine().getReasonPhrase();
                    future.completeExceptionally(new HttpResponseException(statusCode, reasonPhrase));
                    return;
                }

                try {
                    String entity = EntityUtils.toString(response.getEntity());
                    final Map<String, String> map = mapper.readValue(entity, MAP_TYPE);
                    if (map != null && "accepted".equals(map.get("status"))) {
                        future.complete(null);
                    } else {
                        future.completeExceptionally(new BotanException("Unexpected response " + entity));
                    }
                } catch (Exception e) {
                    future.completeExceptionally(e);
                }
            }

            @Override
            public void failed(final Exception e) {
                future.completeExceptionally(e);
            }

            @Override
            public void cancelled() {
                future.completeExceptionally(new InterruptedException());
            }
        });
        return future;
    }
}
