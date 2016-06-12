{-# LANGUAGE DataKinds         #-}
{-# LANGUAGE DeriveAnyClass    #-}
{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE TypeOperators     #-}
{-# LANGUAGE DeriveGeneric     #-}

import           Test.Hspec
import           Data.Text (Text)
import qualified Data.Text as T
import           Network.HTTP.Client      (newManager)
import           Network.HTTP.Client.TLS  (tlsManagerSettings)
import           Servant.Client
import           Web.Botan.Sdk
import           GHC.Generics
import           Data.Aeson

main :: IO ()
main = hspec $
  describe "Botan Client" $ do
    manager <- runIO $ newManager tlsManagerSettings
    it "should track actions" $ do
      let test = toJSON $ Test "A" "B"
      res <- track "token" "user2222" test "test_action" manager
      print res
    it "should shorten url" $ do
      res <- shortenUrl "token" "http://ya.ru" "51123333" manager
      print res

data Test = Test
  {
    a :: Text
  , b :: Text
  } deriving (Show, Generic, ToJSON)