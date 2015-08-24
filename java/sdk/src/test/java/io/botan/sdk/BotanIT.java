package io.botan.sdk;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.apache.http.impl.nio.client.CloseableHttpAsyncClient;
import org.apache.http.impl.nio.client.HttpAsyncClients;
import org.junit.Test;

import java.util.Arrays;

/**
 * @author <a href="mailto:ketoth.xupack@gmail.com">Ketoth Xupack</a>
 * @since 2015-08-25 00:37
 */
public class BotanIT {
    @Test
    public void simpleAPIRequestShouldNotFail() throws Exception {
        try (CloseableHttpAsyncClient client = HttpAsyncClients.createDefault()) {
            client.start();
            Botan botan = new Botan(client, new ObjectMapper());
            botan.track("1111", "1", Arrays.asList(1, 2, 3), "Search").get();
        }
    }
}
