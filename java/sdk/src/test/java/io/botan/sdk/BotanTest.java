package io.botan.sdk;

import com.fasterxml.jackson.core.JsonParseException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.http.HttpEntity;
import org.apache.http.HttpHeaders;
import org.apache.http.HttpResponse;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpResponseException;
import org.apache.http.client.methods.HttpUriRequest;
import org.apache.http.concurrent.FutureCallback;
import org.apache.http.message.BasicHeader;
import org.apache.http.nio.client.HttpAsyncClient;
import org.easymock.Capture;
import org.easymock.EasyMock;
import org.junit.Test;

import java.io.ByteArrayInputStream;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

/**
 * @author <a href="mailto:ketoth.xupack@gmail.com">Ketoth Xupack</a>
 * @since 2015-08-25 00:37
 */
public class BotanTest {
    private static Future<?> testCompleted(final int statusCode, final String body) throws Exception {
        final HttpAsyncClient httpClient = EasyMock.createMock(HttpAsyncClient.class);
        final HttpResponse response = EasyMock.createMock(HttpResponse.class);
        final HttpEntity entity = EasyMock.createMock(HttpEntity.class);
        final StatusLine line = EasyMock.createMock(StatusLine.class);
        EasyMock.expect(response.getStatusLine()).andReturn(line).anyTimes();
        EasyMock.expect(response.getEntity()).andReturn(entity).anyTimes();
        EasyMock.expect(line.getStatusCode()).andReturn(statusCode).anyTimes();
        EasyMock.expect(line.getReasonPhrase()).andReturn("OK").anyTimes();
        final byte[] bytes = body.getBytes("UTF-8");
        EasyMock.expect(entity.getContent()).andReturn(new ByteArrayInputStream(bytes)).anyTimes();
        EasyMock.expect(entity.getContentLength()).andReturn((long) bytes.length).atLeastOnce();
        EasyMock.expect(entity.getContentType())
                .andReturn(new BasicHeader(HttpHeaders.CONTENT_TYPE, "application/json")).anyTimes();

        final Capture<HttpUriRequest> request = new Capture<>();
        final Capture<FutureCallback<HttpResponse>> callback = new Capture<>();
        EasyMock.expect(httpClient.execute(EasyMock.capture(request), EasyMock.capture(callback)))
                .andAnswer(() -> {
                    callback.getValue().completed(response);
                    //noinspection ReturnOfNull
                    return null;
                }).anyTimes();

        EasyMock.replay(httpClient, response, entity, line);
        final Botan botan = new Botan(httpClient, new ObjectMapper());
        return botan.track("token", "userId", new Bean("lollipop", 43), "Message");
    }

    @Test
    public void correctResponseHandled() throws Exception {
        testCompleted(200, "{\"status\": \"accepted\"}").get();
    }

    @Test
    public void invalidJsonHandledCorrectly() throws Exception {
        try {
            testCompleted(200, "blast!").get();
            fail();
        } catch (ExecutionException e) {
            assertTrue(e.getCause() instanceof JsonParseException);
        }
    }

    @Test
    public void unexpectedJsonHandledCorrectly() throws Exception {
        try {
            testCompleted(200, "null").get();
            fail();
        } catch (ExecutionException e) {
            assertTrue(e.getCause() instanceof BotanException);
        }

        try {
            testCompleted(200, "{\"foo\": 1}").get();
            fail();
        } catch (ExecutionException e) {
            assertTrue(e.getCause() instanceof BotanException);
        }
    }

    @Test
    public void non200StatusHandledCorrectly() throws Exception {
        try {
            testCompleted(500, "invalid json").get();
            fail();
        } catch (ExecutionException e) {
            assertTrue(e.getCause() instanceof HttpResponseException);
        }
    }

    public static class Bean {
        private final String type;
        private final int value;

        public Bean(final String type, final int value) {
            this.type = type;
            this.value = value;
        }

        public int getValue() {
            return value;
        }

        public String getType() {
            return type;
        }
    }
}
